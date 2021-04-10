/* 
 *   
 *  Project:          IoT Energy Meter with C/C++, Java/Spring, TypeScript/Angular and Dart/Flutter;
 *  About:            End-to-end implementation of a LoRaWAN network for monitoring electrical quantities;
 *  Version:          1.0;
 *  Backend Mote:     ATmega328P/ESP32/ESP8266/ESP8285/STM32;
 *  Radios:           RFM95w and LoRaWAN EndDevice Radioenge Module: RD49C;
 *  Sensors:          Peacefair PZEM-004T 3.0 Version TTL-RTU kWh Meter;
 *  Backend API:      Java with Framework: Spring Boot;
 *  LoRaWAN Stack:    MCCI Arduino LoRaWAN Library (LMiC: LoRaWAN-MAC-in-C) version 3.0.99;
 *  Activation mode:  Activation by Personalization (ABP) or Over-the-Air Activation (OTAA);
 *  Author:           Adail dos Santos Silva
 *  E-mail:           adail101@hotmail.com
 *  WhatsApp:         +55 89 9 9433-7661
 *  
 *  WARNINGS:
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of
 *  this software and associated documentation files (the “Software”), to deal in
 *  the Software without restriction, including without limitation the rights to
 *  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 *  the Software, and to permit persons to whom the Software is furnished to do so,
 *  subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 *  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 *  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 *  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 *  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *  
 */

/********************************************************************
 _____              __ _                       _   _             
/  __ \            / _(_)                     | | (_)            
| /  \/ ___  _ __ | |_ _  __ _ _   _ _ __ __ _| |_ _  ___  _ __  
| |    / _ \| '_ \|  _| |/ _` | | | | '__/ _` | __| |/ _ \| '_ \ 
| \__/\ (_) | | | | | | | (_| | |_| | | | (_| | |_| | (_) | | | |
 \____/\___/|_| |_|_| |_|\__, |\__,_|_|  \__,_|\__|_|\___/|_| |_|
                          __/ |                                  
                         |___/                                   
********************************************************************/

/* Includes */
#include "_configurations.h"
#include "_credentials.h"
#include "_logs.h"
#include "_uplinks.h"
#include "_downlinks.h"
#include <lmic.h>
#include <SPI.h>

/* Instances */
static osjob_t blinkjob;
static osjob_t sendjob;

/* Pin mapping */
/* LMiC GPIO configuration */
const lmic_pinmap lmic_pins = {
    .nss  = NSS_GPIO,
    .rxtx = 0xFF, /* 0xFF = LMIC_UNUSED_PIN */
    .rst  = RESET_GPIO,
    .dio  = {DIO0_GPIO, DIO1_GPIO, DIO2_GPIO}
};

/*****************************
 _____           _      
/  __ \         | |     
| /  \/ ___   __| | ___ 
| |    / _ \ / _` |/ _ \
| \__/\ (_) | (_| |  __/
 \____/\___/ \__,_|\___|
*****************************/

/* Functions */
/* During the join process, the LED blinks.
 * If joined, the LED stops blinking. 
 */
void blinkfunc(osjob_t *job)
{
    /* Toggle LED */
    joinstatus = !joinstatus;
    switchLed(joinstatus);
    /* Reschedule blink job */
    os_setTimedCallback(job, os_getTime() + ms2osticks(100), blinkfunc);
}

void switchLed(u1_t status)
{
    if (status == 1)
    {
        digitalWrite(LED, HIGH);
    }
    else
    {
        digitalWrite(LED, LOW);
    }
}

void channelsControl()
{
    /* Channels Control */
    for (u1_t b = 0; b < 8; ++b)
    {
        LMIC_disableSubBand(b);
    }
    
    for (u1_t channel = 0; channel < 72; ++channel)
    {
        LMIC_disableChannel(channel);
    }
    
#ifdef USE_ABP
#ifdef USE_CHIRPSTACK_AU915
    /* ChirpStack AU915 */
    LMIC_enableChannel(8);
    LMIC_enableChannel(9);
    LMIC_enableChannel(10);
    LMIC_enableChannel(11);
    LMIC_enableChannel(12);
    LMIC_enableChannel(13);
    LMIC_enableChannel(14);
    LMIC_enableChannel(15);
    LMIC_enableChannel(65); /* Test */
#endif
#ifdef USE_CHIRPSTACK_AU915LA
    /* ChirpStack AU915LA */
    LMIC_enableChannel(0);
    LMIC_enableChannel(1);
    LMIC_enableChannel(2);
    LMIC_enableChannel(3);
    LMIC_enableChannel(4);
    LMIC_enableChannel(5);
    LMIC_enableChannel(6);
    LMIC_enableChannel(7);
    LMIC_enableChannel(64); /* Test */
#endif
#ifdef USE_EVERYNET_AU915LA
    /* Rede ATC (Everynet) AU915LA */
    LMIC_enableChannel(0);
    LMIC_enableChannel(1);
    LMIC_enableChannel(2);
    LMIC_enableChannel(3);
    LMIC_enableChannel(4);
    LMIC_enableChannel(5);
    LMIC_enableChannel(6);
    LMIC_enableChannel(7);
#endif
#ifdef USE_THETHINGSNETWORK_AU915
    /* Rede TTN AU915 */
    LMIC_enableChannel(8);
    LMIC_enableChannel(9);
    LMIC_enableChannel(10);
    LMIC_enableChannel(11);
    LMIC_enableChannel(12);
    LMIC_enableChannel(13);
    LMIC_enableChannel(14);
    LMIC_enableChannel(15);
    LMIC_enableChannel(65); /* Test */
#endif
#endif

#ifdef USE_OTAA
#ifdef USE_CHIRPSTACK_AU915
    /* ChirpStack AU915 */
    LMIC_enableChannel(8);
    LMIC_enableChannel(9);
    LMIC_enableChannel(10);
    LMIC_enableChannel(11);
    LMIC_enableChannel(12);
    LMIC_enableChannel(13);
    LMIC_enableChannel(14);
    LMIC_enableChannel(15);
    LMIC_enableChannel(65); /* Test */
#endif
#ifdef USE_CHIRPSTACK_AU915LA
    /* ChirpStack AU915LA */
    LMIC_enableChannel(0);
    LMIC_enableChannel(1);
    LMIC_enableChannel(2);
    LMIC_enableChannel(3);
    LMIC_enableChannel(4);
    LMIC_enableChannel(5);
    LMIC_enableChannel(6);
    LMIC_enableChannel(7);
    LMIC_enableChannel(64); /* Test */
#endif
#ifdef USE_EVERYNET_AU915LA
    /* Rede ATC (Everynet) AU915LA */
    LMIC_enableChannel(0);
    LMIC_enableChannel(1);
    LMIC_enableChannel(2);
    LMIC_enableChannel(3);
    LMIC_enableChannel(4);
    LMIC_enableChannel(5);
    LMIC_enableChannel(6);
    LMIC_enableChannel(7);
#endif
#ifdef USE_THETHINGSNETWORK_AU915
    /* Rede TTN AU915 */
    LMIC_enableChannel(8);
    LMIC_enableChannel(9);
    LMIC_enableChannel(10);
    LMIC_enableChannel(11);
    LMIC_enableChannel(12);
    LMIC_enableChannel(13);
    LMIC_enableChannel(14);
    LMIC_enableChannel(15);
    LMIC_enableChannel(65); /* Test */
#endif
#endif

#ifdef GATEWAY_SINGLE_CHANNEL
    /* Channels Control */
    for (u1_t b = 0; b < 8; ++b)
    {
        LMIC_disableSubBand(b);
    }
    
    for (u1_t channel = 0; channel < 72; ++channel)
    {
        LMIC_disableChannel(channel);
    }
    LMIC_enableChannel(SINGLE_CHANNEL);
#endif
}

/* LMiC Events */
void onEvent(ev_t ev)
{
    #ifdef DEBUG
    DEBUG_PORT.print(os_getTime());
    DEBUG_PORT.print(": ");
    #endif
    switch (ev)
    {
    case EV_SCAN_TIMEOUT:
        #ifdef DEBUG
        DEBUG_PORT.println(F("EV_SCAN_TIMEOUT"));
        #endif
        break;
    case EV_BEACON_FOUND:
        #ifdef DEBUG
        DEBUG_PORT.println(F("EV_BEACON_FOUND"));
        #endif
        break;
    case EV_BEACON_MISSED:
        #ifdef DEBUG
        DEBUG_PORT.println(F("EV_BEACON_MISSED"));
        #endif
        break;
    case EV_BEACON_TRACKED:
        #ifdef DEBUG
        DEBUG_PORT.println(F("EV_BEACON_TRACKED"));
        #endif
        break;
    case EV_JOINING:
        #ifdef DEBUG
        DEBUG_PORT.println(F("EV_JOINING"));
        #endif
        
        /* Start blinking */
        blinkfunc(&blinkjob);
        break;
    case EV_JOINED:
        #ifdef DEBUG
        DEBUG_PORT.println(F("EV_JOINED"));
        #endif
        
        /* Variable to Log TX or RX */
        modeOperation = "RX";
        
        showNetworkInformations();
        
        showTxRxInformations();
        
        /* Switch Led off */
        switchLed(0);
        
        /* Cancel blink job */
        os_clearCallback(&blinkjob);
        
        /* Downlink datarate */
        /* The Things Networks uses SF9 for its RX2 window */
        LMIC.dn2Dr = DN2DR;
        
        LMIC_setAdrMode(ADR_MODE);
        
        /* 
         *  Set data rate and transmit power for uplink 
         *  (note: txpow seems to be ignored by the library) 
         */
        if (ADR_MODE != 1)
        {
            LMIC_setDrTxpow(UPLINK_DATA_RATE, TRANSMIT_POWER);
        }
        
        /*  Enable or Disable link check validation
         *  Disable link check validation (automatically enabled 
         *  during join, but not supported by The Things Network at this time).
         */
        LMIC_setLinkCheckMode(LINK_CHECK_MODE);
        
        /* Channels Control */
        channelsControl(); /* Just to confirm, already called in setup() */
        
        /* Downlinks Control Time */
        downlinksControlTime();
        break;
    case EV_RFU1:
        #ifdef DEBUG
        DEBUG_PORT.println(F("EV_RFU1"));
        #endif
        break;
    case EV_JOIN_FAILED:
        #ifdef DEBUG
        DEBUG_PORT.println(F("EV_JOIN_FAILED"));
        #endif
        break;
    case EV_REJOIN_FAILED:
        #ifdef DEBUG
        DEBUG_PORT.println(F("EV_REJOIN_FAILED"));
        #endif
        break;
        break;
    case EV_TXCOMPLETE:
        #ifdef DEBUG
        DEBUG_PORT.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
        #endif
                
        /* Downlinks Log */
        downlinksLog();
        
        /* Downlink rules */
        downlinksRule();
        
        /* Variable to Log TX or RX */
        modeOperation = "RX";
        
        /* Logs TX and RX */
        showTxRxInformations();
                
        /* Schedule next transmission */
        os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL), do_send);
        break;
    case EV_LOST_TSYNC:
        #ifdef DEBUG
        DEBUG_PORT.println(F("EV_LOST_TSYNC"));
        #endif
        break;
    case EV_RESET:
        #ifdef DEBUG
        DEBUG_PORT.println(F("EV_RESET"));
        #endif
        break;
    case EV_RXCOMPLETE:
        /* Data received in ping slot */
        #ifdef DEBUG
        DEBUG_PORT.println(F("EV_RXCOMPLETE"));
        #endif
        break;
    case EV_LINK_DEAD:
        #ifdef DEBUG
        DEBUG_PORT.println(F("EV_LINK_DEAD"));
        #endif
        break;
    case EV_LINK_ALIVE:
        #ifdef DEBUG
        DEBUG_PORT.println(F("EV_LINK_ALIVE"));
        #endif
        break;
    default:
        #ifdef DEBUG
        DEBUG_PORT.println(F("Unknown event"));
        #endif
        break;
    }
}

/* 
 *  Sensitive function regarding TX and RX1 / 2 time synchronization, do not
 *  implement anything unnecessary inside, prioritize only the sending of uplinks.
 */
void do_send(osjob_t *j)
{
    /* Check if there is not a current TX/RX job running */
    if (LMIC.opmode & OP_TXRXPEND)
    {
        #ifdef DEBUG
        DEBUG_PORT.println(F("OP_TXRXPEND, not sending"));
        #endif
    }
    else
    {
        /* Send LoRa Packet */        
        /* Calls uplink sending function */
        payloadMyNameVerticalBytes();
        
        /* Variable to Log TX or RX */
        modeOperation = "TX";

        /* 
         *  WARNING:
         *  Calling Serial too much at this point in the execution 
         *  causes loss of time synchronization with Downlink windows!
         */
//        showTxRxInformations();
        
        #ifdef DEBUG
        DEBUG_PORT.println(" [INFO] " + modeOperation + " on Frequency     : " + String(LMIC.freq / 1000000.0) + " MHz, Packet queued.");
//        DEBUG_PORT.println(F(" [INFO] Packet queued"));
        #endif
        
        /* Counters Control */
        seqNoUp = LMIC.seqnoUp;
//        seqNoDn = LMIC.seqnoDn;
    }
    /* Next TX is scheduled after TX_COMPLETE event */
}

/*****************************
 _____      _               
/  ___|    | |              
\ `--.  ___| |_ _   _ _ __  
 `--. \/ _ \ __| | | | '_ \ 
/\__/ /  __/ |_| |_| | |_) |
\____/ \___|\__|\__,_| .__/ 
                     | |    
                     |_|    
******************************/

void setup()
{
  
/* Debug */
#ifdef DEBUG_PORT
    while (!DEBUG_PORT); // Wait for DEBUG_PORT to be initialized
    DEBUG_PORT.begin(SERIAL_BAUD_RATE);
#endif

    #ifdef DEBUG
    DEBUG_PORT.println(F("Starting"));
    #endif
    
    pinMode(LED, OUTPUT);
    
#ifdef VCC_ENABLE
    /* For Pinoccio Scout boards */
    pinMode(VCC_ENABLE, OUTPUT);
    digitalWrite(VCC_ENABLE, HIGH);
    delay(1000);
#endif
    
    /* LMIC init */
    os_init();
    
    /* Reset the MAC state. Session and pending data transfers will be discarded. */
    LMIC_reset();
    
/* Activation by Personalization (ABP) */
#ifdef USE_ABP
/*
 *  Set static session parameters. Instead of dynamically establishing a session
 *  by joining the network, precomputed session parameters are be provided. 
 */
#ifdef PROGMEM
    /*
     *  On AVR, these values are stored in flash and only copied to RAM once.
     *  Copy them to a temporary buffer here, LMIC_setSession will.
     *  Copy them into a buffer of its own again.
     */
    uint8_t appskey[sizeof(APPSKEY)];
    uint8_t nwkskey[sizeof(NWKSKEY)];
    memcpy_P(appskey, APPSKEY, sizeof(APPSKEY));
    memcpy_P(nwkskey, NWKSKEY, sizeof(NWKSKEY));
    LMIC_setSession(0x1, DEVADDR, nwkskey, appskey);
#else
    /* If not running an AVR with PROGMEM, just use the arrays directly */
    LMIC_setSession(0x1, DEVADDR, NWKSKEY, APPSKEY);
#endif
    
    /* Downlink datarate */
    /* The Things Networks uses SF9 for its RX2 window */
    LMIC.dn2Dr = DN2DR;
    
    LMIC_setAdrMode(ADR_MODE);
    
    /* 
     *  Set data rate and transmit power for uplink 
     *  (note: txpow seems to be ignored by the library) 
     */
    if (ADR_MODE != 1)
    {
        LMIC_setDrTxpow(UPLINK_DATA_RATE, TRANSMIT_POWER);
    }
    
    /* Enable or Disable link check validation */
    LMIC_setLinkCheckMode(LINK_CHECK_MODE);
    
    /* Channels Control */
    channelsControl();
    
    /* Downlinks Control Time */
    downlinksControlTime();
#endif

/* Over-the-Air Activation (OTAA) */
#ifdef USE_OTAA
#ifdef USE_FORCE_OTAA
    /* Setando para iniciar rapidamento o Join OTAA, não entrará no EV_JOINING */
    LMIC_startJoining();
    LMIC.txChnl = USE_CHANNEL_START_JOINING;
#endif
    /* --> Next EV_JOINING */
    /* Channels Control */
    channelsControl();
    
    /* 
     *  When in OTAA mode and reaches that point, 
     *  the event is immediately called: EV_JOINING.
     */    
#endif

    /* When you get here go to EV */
    do_send(&sendjob);
}

/*****************************
 _                       
| |                      
| |     ___   ___  _ __  
| |    / _ \ / _ \| '_ \ 
| |___| (_) | (_) | |_) |
\_____/\___/ \___/| .__/ 
                  | |    
                  |_|    
*****************************/

void loop()
{
    /* Loop once only */
    os_runloop_once();
}
