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

#pragma once

/* Payloads */
//uint8_t payload_HexadecimalValue_101[1] = { 0x65 }; /* Test - Send Decimal: 101 - ASCII: "e" */

/* Only accepts decimal values */
//byte payloadMyNameHorizontalBytes[10] = { 65, 100, 97, 105, 108, 83, 105, 108, 118, 97 };

/* Relay uplink - Ok */
byte payloadRelayUplink[17] = { 82, 101, 108, 97, 121, 32, 117, 112, 108, 105, 110, 107, 32, 45, 32, 79, 107 };

/* Send Functions */

/* Shipments - Byte uploads */
/* Calls uplink sending function */
void payloadSend(uint8_t port, uint8_t * data, uint8_t data_size, bool confirmed)
{
    /*
     *  Prepare upstream data transmission at the next possible time.
     *  Parameters are port, data, length, confirmed.
     */
    LMIC_setTxData2(port, data, data_size, confirmed ? 1 : 0);
}

/* Shipments - Byte uploads */
/* Calls uplink sending function */
void payloadMyNameVerticalBytes()
{
    /*
     *  Payload My Name
     *  ASCII text:  AdailSilva
     *  Base64:      QWRhaWxTaWx2YQ==
     *  Hexadecimal: 41 64 61 69 6C 53 69 6C 76 61
     *  Decimal:     65 100 97 105 108 83 105 108 118 97
     *  Binary:      01000001 01100100 01100001 01101001 01101100 01010011 01101001 01101100 01110110 01100001
     */
    byte payload[10];

    payload[0] = 65;
    payload[1] = 100;
    payload[2] = 97;
    payload[3] = 105;
    payload[4] = 108;
    payload[5] = 83;
    payload[6] = 105;
    payload[7] = 108;
    payload[8] = 118;
    payload[9] = 97;

    /* Direct Transmission LMiC */
    /*
     *  Prepare upstream data transmission at the next possible time.
     *  Parameters are port, data, length, confirmed.
     */
     LMIC_setTxData2(UPLINK_PORT, payload, sizeof(payload), UPLINK_CONFIRMED);
}
