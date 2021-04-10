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

/*
 *  Important Tools for Working with LoRaWAN
 * 
 *  Airtime calculator          - https://www.thethingsnetwork.org/airtime-calculator
 *  LoraW packet dec            - https://lorawan-packet-decoder-0ta6puiniaut.runkit.sh/?data
 *  RF unit converter           - https://www.eeweb.com/tools/rf-unit-converter/
 *  Coax calculator             - https://www.eeweb.com/tools/coax
 *  dBm conversion              - https://www.rapidtables.com/convert/power/dBm_to_Watt.html
 *  Return loss                 - https://www.ubcorp.com/return-loss-vs-voltage-standing.../
 *  JPole calculator            - http://www.hamuniverse.com/jpole.html
 *  RF propagation              - http://radiomobile.pe1mew.nl
 *  Calculate distance          - http://www.movable-type.co.uk/scripts/latlong.html
 *  Open port checker           - https://www.yougetsignal.com/tools/open-ports/
 *  IPv6 checker                - http://test-ipv6.com/
 *  Wire Gauge conv             - https://www.engineeringtoolbox.com/awg-wire-gauge-d_731.html
 *  EASY EDA                    - https://easyeda.com/editor
 *  Circuit sim                 - http://lushprojects.com/circuitjs/
 *  Tone Generator              - https://onlinetonegenerator.com/
 *  Calculate LM317             - http://www.muzique.com/schem/lm317.htm
 *  Battery life calcu          - http://oregonembedded.com/batterycalc.htm
 *  Voltage drop                - https://www.calculator.net/voltage-drop-calculator.html
 *  Bin to Hex                  - https://www.mathsisfun.com/binary-decimal-hexadecimal...
 *  Dec to Bin                  - https://www.rapidtables.com/.../decimal-to-binary.html
 *  Hex to Dec                  - https://www.binaryhexconverter.com/hex-to-decimal-converter
 *  Hex to Bin                  - https://www.binaryhexconverter.com/hex-to-binary-converter
 *  Hex to Base64               - https://base64.guru/converter/encode/hex
 *  Base64 to ASCII             - https://www.base64decode.org/
 *  Resistive devider           - http://www.magic-worlld.narod.ru/
 *  BMP converter               - https://products.groupdocs.app/metadata/bmp
 *  Epub converter              - https://www.convertfiles.com/convert/ebook/EPUB-to-PDF.html
 *  CODEPILE                    - https://www.codepile.net
 *  CODESHARE                   - https://codeshare.io/
 *  RequestsBin.NET             - http://requestbin.net/
 *  JSON Formatter & Validator  - https://jsonformatter.curiousconcept.com/
 *  JSON for in line String     - https://tools.knowledgewalls.com/jsontostring
 *  
 *  Info:
 *  SNR                         - https://en.wikipedia.org/wiki/Signal-to-noise_ratio
 *  RSSI                        - https://en.wikipedia.org/.../Received_signal_strength...
 *  DBI                         - https://en.wikipedia.org/wiki/Antenna_gain
 *  
 *  Simulation of radio propagation (also useful for LoRaWAN)
 *  https://link.ui.com/?fbclid=IwAR12Lh9sqspdMmBwyCiI3sjtFzMB80ZlnONlck40ltbYA48uNi6li35E5Uo
 *  
 */

/*
 *  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
 *  Nodes in The Things Network - TTN
 *  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 *  # OTAA
 *  Device ID       ttgo_tbeam_otaa
 *  Description     TTGO TBeam OTAA
 *  Device EUI      00275bef0fbde4f0
 *  Application EUI 70b3d57ed0031701
 *  App Key         70dca98078374c51633e4d7316a6b3b1
 *  ------------------------------------------------
 *  # ABP
 *  ttgo_tbeam_abp
 *  Description         TTGO TBeam ABP
 *  Device EUI          00275bef0fbde4f0
 *  Application EUI     70b3d57ed0031701
 *  App Key             70dca98078374c51633e4d7316a6b3b1
 *  
 *  Device Address      26031738 // Variable
 *  Network Session Key a59b565ed0c708cc2bf27d8b7140ed90
 *  App Session Key     f358b74f3603036b463ec486dbb22955
 *  ----------------------------------------------------
 *  
 *  %%%%%%%%%%%%%%%%%%%%%%%%%
 *  Nodes in ChirpStack - CS
 *  %%%%%%%%%%%%%%%%%%%%%%%%%
 * 
 */
  
/* 
 *  To view logs at the gateways
 *  tail -f /var/log/syslog | grep "JSON up"
 *  tail -f /var/log/syslog | grep "JSON down"
 */

/*
 *  IN GATEWAY
 *  
 *  ### Downlinks OTAA:
 *  
 *  JSON up: {"time":"2020-12-25T18:28:46Z","rxpk":[{"tmst":3635223316,"time":"2020-12-25T18:28:46.704289Z","chan":5,"rfch":1,"freq":917.800000,"stat":1,"modu":"LORA","datr":"SF10BW125","codr":"4/5","lsnr":9.2,"rssi":-25,"size":23,"data":"AAEXA9B+1bNw8OS9D+9bJwBFN37uDrU="}]}
 *  
 *  5000000
 *  
 *  JSON down: {"txpk":{"imme":false,"tmst":3640223316,"freq":926.3,"rfch":0,"powe":20,"modu":"LORA","datr":"SF10BW500","codr":"4/5","ipol":true,"size":17,"ncrc":true,"data":"IFskvyGfiBxj7+un82Xh0PQ="}}
 *  
 *  1184543 - Calibrate this moment...
 *  
 *  JSON up: {"time":"2020-12-25T18:28:52Z","rxpk":[{"tmst":3641407859,"time":"2020-12-25T18:28:52.888912Z","chan":3,"rfch":0,"freq":917.400000,"stat":1,"modu":"LORA","datr":"SF7BW125","codr":"4/5","lsnr":9.0,"rssi":-31,"size":23,"data":"QMUmAyYAAABll2Y8tWcHNMvlsvlWalY="}]}
 *  
 *  3 640 223 316 - 3 635 223 316 = 5 000 000 (5000000)
 *  3 641 407 859 - 3 640 223 316 = 1 184 543 (1184543)
 *  
 *  ### Downlinks Comuns:
 *  
 *  JSON up: {"time":"2020-12-25T18:36:50Z","rxpk":[{"tmst":4119158379,"time":"2020-12-25T18:36:50.645765Z","chan":4,"rfch":1,"freq":917.600000,"stat":1,"modu":"LORA","datr":"SF7BW125","codr":"4/5","lsnr":9.5,"rssi":-33,"size":23,"data":"QOAgAyYAAgBlOaVbpuXsD3kxT2ppQUw="}]}
 *  
 *  1000000
 *  
 *  JSON down: {"txpk":{"imme":false,"tmst":4120158379,"freq":925.7,"rfch":0,"powe":20,"modu":"LORA","datr":"SF7BW500","codr":"4/5","ipol":true,"size":14,"ncrc":true,"data":"YOAgAyYAAAABXKj0hwQ="}}
 *  
 *  4 120 158 379 - 4 119 158 379 = 1 000 000 (1000000)
 *
 */

/*
 *  IN NODE
 *  
 *  ### CLOCK_ERROR 0 %:
 *  
 *  ### Join Request - OTAA:
 *  56697: TXMODE, freq=917200000, len=23, SF=10, BW=125, CR=4/5, IH=0
 *  
 *  start single rx: now-rxtime: 3
 *  392256: RXMODE_SINGLE, freq=924500000, SF=10, BW=500, CR=4/5, IH=0
 *  
 *  ### Time Join Response - OTAA:
 *  392 256 - 56 697 = 335 559
 *  
 *  ### Time Downlinks:
 *  463576: TXMODE, freq=917400000, len=23, SF=7, BW=125, CR=4/5, IH=0
 *  
 *  start single rx: now-rxtime: 4
 *  530320: RXMODE_SINGLE, freq=925100000, SF=7, BW=500, CR=4/5, IH=0
 *  rxtimeout: entry: 530547 rxtime: 530312 entry-rxtime: 235 now-entry: 4 rxtime-txend: 62375
 *  
 *  start single rx: now-rxtime: 4
 *  592819: RXMODE_SINGLE, freq=923300000, SF=9, BW=125, CR=4/5, IH=0
 *  rxtimeout: entry: 594627 rxtime: 592812 entry-rxtime: 1815 now-entry: 4 rxtime-txend: 124875
 *  
 *  ### Time to Open RX1:
 *  530 320 - 463 576 = 66 744  (Tx   -->   Rx1 time  : 1 000 000)
 *  
 *  ### Time to Open RX2:
 *  592 819 - 530 320 = 62 499  (Rx1  -->   Rx2 time  : 1 000 000) --> (Duration Rx1)
 *  592 819 - 463 576 = 129 243 (TX   -->   Rx2 time  : 2 000 000)
 *  
 *  
 *  
 *  ### CLOCK_ERROR 10 %:
 *  
 *  ### Join Request - OTAA:
 *  56702: TXMODE, freq=918200000, len=23, SF=10, BW=125, CR=4/5, IH=0
 *  
 *  start single rx: now-rxtime: 3
 *  392262: RXMODE_SINGLE, freq=927500000, SF=10, BW=500, CR=4/5, IH=0
 *  
 *  ### Time Join Response - OTAA:
 *  392 262 - 56 702 = 335 560
 *  
 *  ### Time Downlinks:
 *  463582: TXMODE, freq=916800000, len=23, SF=7, BW=125, CR=4/5, IH=0
 *  
 *  start single rx: now-rxtime: 4
 *  530077: RXMODE_SINGLE, freq=923300000, SF=7, BW=500, CR=4/5, IH=0
 *  rxtimeout: entry: 530800 rxtime: 530069 entry-rxtime: 731 now-entry: 4 rxtime-txend: 62126
 *  
 *  start single rx: now-rxtime: 4
 *  592326: RXMODE_SINGLE, freq=923300000, SF=9, BW=125, CR=4/5, IH=0
 *  rxtimeout: entry: 595158 rxtime: 592319 entry-rxtime: 2839 now-entry: 4 rxtime-txend: 124376
 *  
 *  ### Time to Open RX1:
 *  530 077 - 463 582 = 66 495  (Tx --> Rx1 time  : 1 000 000)
 *  
 *  ### Time to Open RX2:
 *  592 326 - 530 077 = 62 249  (Rx1  -->   Rx2 time  : 1 000 000) --> (Duration Rx1)
 *  592 326 - 463 582 = 128 744 (TX   -->   Rx2 time  : 2 000 000)
 *  
 */

 /*
  *   AU915-928 in The Things Network - TTN
  *   
  *   Uplink:
  *   916.8 - SF7BW125 to SF12BW125
  *   917.0 - SF7BW125 to SF12BW125
  *   917.2 - SF7BW125 to SF12BW125
  *   917.4 - SF7BW125 to SF12BW125
  *   917.6 - SF7BW125 to SF12BW125
  *   917.8 - SF7BW125 to SF12BW125
  *   918.0 - SF7BW125 to SF12BW125
  *   918.2 - SF7BW125 to SF12BW125
  *   917.5 - SF8BW500
  *   
  *   Downlink:
  *   923.3 - SF7BW500 to SF12BW500 (RX1)
  *   923.9 - SF7BW500 to SF12BW500 (RX1)
  *   924.5 - SF7BW500 to SF12BW500 (RX1)
  *   925.1 - SF7BW500 to SF12BW500 (RX1)
  *   925.7 - SF7BW500 to SF12BW500 (RX1)
  *   926.3 - SF7BW500 to SF12BW500 (RX1)
  *   926.9 - SF7BW500 to SF12BW500 (RX1)
  *   927.5 - SF7BW500 to SF12BW500 (RX1)
  *   923.3 - SF12BW500 (RX2)
  *   
  *   Note that The Things Network uses 2nd Sub-Band only (channels 8 to 15 and 65).
  *   You’ll need to program the specific channels into the devices in order to make them work with TTN.
  * 
  */  

/* A DETAIL FOR THE LoRaWAN PROTOCOL:
 * 
 * For the First Reception Window, each DOWNLINK frequency is heard by the node, in a channel whose index of the same,
 * taking into account the plan used, it will correspond to the channel index of the last UPLINK frequency sent by the node.
 *
 * For the second Reception Window, it will always be active to listen to the default channel for Downlinks 0, which has a frequency of 923.3MHz.
 * This channel (frequency) of the second reception window can be customized, respecting the downlink frequencies of the chosen plan.
 *
 * Regarding the order of channels, it also applies to the sending rules (TX) by the Gateway at the request of the Network Server to the nodes.
 *
 * 
 * EXAMPLE FOR THE AU915-928 FREQUENCY PLAN:
 * 
 * "UPLINK at 916.8MHz then Listen DOWNLINK at: RX1 923.3MHz RX2 923.3MHz. (Customizable RX2)";
 *
 * "UPLINK at 918.2MHz then Listen DOWNLINK at: RX1 927.5MHz RX2 923.3MHz. (Customizable RX2)"; 
 * 
 *  
 * | Channel | Direction | Frequency (MHz) | Bandwidth (kHz) |  Data Rate |
 * |:-------:|:---------:|:---------------:|:---------------:|:----------:|
 * |    8    |     UP    |      916.8      |       125       |  DR0 - DR3 |
 * |    9    |     UP    |      917.0      |       125       |  DR0 - DR3 |
 * |    10   |     UP    |      917.2      |       125       |  DR0 - DR3 |
 * |    11   |     UP    |      917.4      |       125       |  DR0 - DR3 |
 * |    12   |     UP    |      917.6      |       125       |  DR0 - DR3 |
 * |    13   |     UP    |      917.8      |       125       |  DR0 - DR3 |
 * |    14   |     UP    |      918.0      |       125       |  DR0 - DR3 |
 * |    15   |     UP    |      918.2      |       125       |  DR0 - DR3 |
 * |    65   |     UP    |      917.5      |       500       |     DR4    |
 * |    0    |    DOWN   |      923.3      |       500       | DR8 - DR13 |
 * |    1    |    DOWN   |      923.9      |       500       | DR8 - DR13 |
 * |    2    |    DOWN   |      924.5      |       500       | DR8 - DR13 |
 * |    3    |    DOWN   |      925.1      |       500       | DR8 - DR13 |
 * |    4    |    DOWN   |      925.7      |       500       | DR8 - DR13 |
 * |    5    |    DOWN   |      926.3      |       500       | DR8 - DR13 |
 * |    6    |    DOWN   |      926.9      |       500       | DR8 - DR13 |
 * |    7    |    DOWN   |      927.5      |       500       | DR8 - DR13 |
 * 
 */

 /*
  *   ChirpStack MQTT Commands
  *   
  *   ### Uplinks
  *   By Application:
  *   application/1/device/+/event/up
  *   
  *   By Device:
  *   application/1/device/de631ea3e5f58df3/event/up
  *   
  *   ### Downlinks
  *   By Application:
  *   [não se aplica]
  *   
  *   By Device:
  *   application/1/device/de631ea3e5f58df3/command/down
  *   
  *   Payload:
  *   HEX           BASE64      DEC
  *   00            AA==        0
  *   01            AQ==        1
  *   65            ZQ==        101
  *   5501001eff    VQEAHv8=    85 1 0 30 61440
  *   55020000ff    VQIAAP8=    85 2 0  0 61440
  *   
  *   
  *   
  *   ### Mosquitto in Terminal:
  *   mosquitto_sub -h localhost -t "test/adailsilva/101" -u "adailsilva" -P "@Hacker101"
  *   mosquitto_pub -h localhost -t "test/adailsilva/101" -m "AdailSilva" -u "adailsilva" -P "@Hacker101"
  *   
  *   
  *   ### Mosquitto in Terminal to ChirpStack:  (SUBSCRIBE)
  *   mosquitto_sub -t "application/123/#" -v                  # Display everything for the given application ID
  *   mosquitto_sub -t "application/123/device/+/event/up" -v  # Display only the uplink payloads for the given application ID
  *   
  *   Any device event:
  *   mosquitto_sub -t "application/2/#" -h localhost -u "adailsilva" -P "@Hacker101" -v
  *   mosquitto_sub -t "application/2/device/+/event/#" -h localhost -u "adailsilva" -P "@Hacker101" -v
  *   
  *   
  *   #################
  *   Examples UPLINKS:
  *   mosquitto_sub -t "application/2/device/+/event/up" -h localhost -u "adailsilva" -P "@Hacker101" -v
  *   mosquitto_sub -t "application/2/device/de631ea3e5f58df3/event/up" -h localhost -u "adailsilva" -P "@Hacker101" -v
  *   
  *   Examples STATUS:  (I didn't capture anything here)
  *   mosquitto_sub -t "application/2/device/+/event/status" -h localhost -u "adailsilva" -P "@Hacker101" -v
  *   mosquitto_sub -t "application/2/device/de631ea3e5f58df3/event/status" -h localhost -u "adailsilva" -P "@Hacker101" -v
  *   
  *   Examples JOIN:
  *   mosquitto_sub -t "application/2/device/+/event/join" -h localhost -u "adailsilva" -P "@Hacker101" -v
  *   
  *   Examples ACK:
  *   mosquitto_sub -t "application/2/device/+/event/ack" -h localhost -u "adailsilva" -P "@Hacker101" -v
  *   
  *   Examples TXACK:
  *   mosquitto_sub -t "application/2/device/+/event/txack" -h localhost -u "adailsilva" -P "@Hacker101" -v
  *   
  *   Examples ERROR:
  *   mosquitto_sub -t "application/2/device/+/event/error" -h localhost -u "adailsilva" -P "@Hacker101" -v
  *   
  *   
  *   ### Mosquitto in Terminal to ChirpStack:  (PUBLISH)
  *   ################################
  *   Examples Scheduling a DOWNLINKS:
  *   The default topic for scheduling downlink payloads is:
  *   application/[ApplicationID]/device/[DevEUI]/command/down.
  *   
  *   Before v3.11.0, the default event topic was:
  *   application[ApplicationID]/device/[DevEUI]/tx.
  *   In case these are configured in the ChirpStack Application Server 
  *   configuration, then these will override the default configuration.
  *   
  *   
  *   {
  *       "confirmed": true,                            // Whether the payload must be sent as confirmed data down or not
  *       "fPort": 10,                                  // FPort to use (must be > 0)
  *       "data": "....",                               // Base64 encoded data (plaintext, will be encrypted by ChirpStack Network Server)
  *       "object": {                                   // Decoded object (when application coded has been configured)
  *                     "temperatureSensor": {"1": 25}, // When providing the 'object', you can omit 'data'
  *                     "humiditySensor": {"1": 32}
  *                 }
  *   }
  *   
  *   
  *   {
  *       "confirmed":true,
  *       "fPort":10,
  *       "data":"QWRhaWxTaWx2YQ=="
  *   }
  *   
  *   {
  *       "confirmed":true,
  *       "fPort":10,
  *       "data":"QWRhaWxTaWx2YQ==",
  *       "object": {
  *                     "temperatureSensor":{"1":25},
  *                     "humiditySensor":{"1":32}
  *                 }
  *   }
  *   
  *   
  *   Examples:
  *   
  *   {
  *       "confirmed": false,
  *       "fPort": 101,
  *       "data": "AQ=="
  *   }
  *   
  *   "{\"confirmed\":false,\"fPort\":101,\"data\":\"AQ==\"}"
  *   
  *   {
  *       "confirmed": false,
  *       "fPort": 101,
  *       "data": "AA=="
  *   }
  *   
  *   "{\"confirmed\":false,\"fPort\":101,\"data\":\"AA==\"}"
  *   
  *   {
  *       "confirmed": true,
  *       "fPort": 101,  
  *       "data": "AQ=="
  *   }
  *   
  *   "{\"confirmed\":true,\"fPort\":101,\"data\":\"AQ==\"}"
  *   
  *   {
  *       "confirmed": true,
  *       "fPort": 101,
  *       "data": "AA=="
  *   }
  *   
  *   "{\"confirmed\":true,\"fPort\":101,\"data\":\"AA==\"}"
  *   
  *   {
  *       "confirmed": false,
  *       "fPort": 101,
  *       "data": "AQ==",
  *       "object": {
  *                     "temperatureSensor": {"1": 25},
  *                     "humiditySensor": {"1": 32}
  *                 }
  *   }
  *   
  *   "{\"confirmed\":false,\"fPort\":101,\"data\":\"AQ==\",\"object\":{\"temperatureSensor\":{\"1\":25},\"humiditySensor\":{\"1\":32}}}"
  *   
  *   
  *   {
  *       "confirmed": false,
  *       "fPort": 101,
  *       "data": "AA==",
  *       "object": {
  *                     "temperatureSensor": {"1": 25},
  *                     "humiditySensor": {"1": 32}
  *                 }
  *   }
  *   
  *   "{\"confirmed\":false,\"fPort\":101,\"data\":\"AA==\",\"object\":{\"temperatureSensor\":{\"1\":25},\"humiditySensor\":{\"1\":32}}}"
  *   
  *   {
  *       "confirmed": true,
  *       "fPort": 101,
  *       "data": "AQ==",
  *       "object": {
  *                     "temperatureSensor": {"1": 25},
  *                     "humiditySensor": {"1": 32}
  *                 }
  *   }
  *   
  *   "{\"confirmed\":true,\"fPort\":101,\"data\":\"AQ==\",\"object\":{\"temperatureSensor\":{\"1\":25},\"humiditySensor\":{\"1\":32}}}"
  *   
  *   {
  *       "confirmed": true,
  *       "fPort": 101,
  *       "data": "AA==",
  *       "object": {
  *                   "temperatureSensor": {"1": 25},
  *                   "humiditySensor": {"1": 32}
  *                 }
  *   }
  *   
  *   "{\"confirmed\":true,\"fPort\":101,\"data\":\"AA==\",\"object\":{\"temperatureSensor\":{\"1\":25},\"humiditySensor\":{\"1\":32}}}"
  *   
  *   
  *   
  *   No Object, Unconfirmed Downlinks:
  *   
  *   ### Light up, TX topic:
  *   mosquitto_pub -t "application/2/device/de631ea3e5f58df3/command/tx" -h localhost -u "adailsilva" -P "@Hacker101" -m "{\"confirmed\":false,\"fPort\":101,\"data\":\"AQ==\"}"
  *   
  *   ### Light up, DOWN topic:
  *   mosquitto_pub -t "application/2/device/de631ea3e5f58df3/command/down" -h localhost -u "adailsilva" -P "@Hacker101" -m "{\"confirmed\":false,\"fPort\":101,\"data\":\"AQ==\"}"
  *   
  *   ### Disconnect, TX topic:
  *   mosquitto_pub -t "application/2/device/de631ea3e5f58df3/command/tx" -h localhost -u "adailsilva" -P "@Hacker101" -m "{\"confirmed\":false,\"fPort\":101,\"data\":\"AA==\"}"
  *   
  *   ### Disconnect, DOWN topic:
  *   mosquitto_pub -t "application/2/device/de631ea3e5f58df3/command/down" -h localhost -u "adailsilva" -P "@Hacker101" -m "{\"confirmed\":false,\"fPort\":101,\"data\":\"AA==\"}"
  *   
  *   
  *   No Object, Confirmed Downlinks:
  *   
  *   ### Light up, TX topic:
  *   mosquitto_pub -t "application/2/device/de631ea3e5f58df3/command/tx" -h localhost -u "adailsilva" -P "@Hacker101" -m "{\"confirmed\":true,\"fPort\":101,\"data\":\"AQ==\"}"
  *   
  *   ### Light up, DOWN topic:
  *   mosquitto_pub -t "application/2/device/de631ea3e5f58df3/command/down" -h localhost -u "adailsilva" -P "@Hacker101" -m "{\"confirmed\":true,\"fPort\":101,\"data\":\"AQ==\"}"
  *   
  *   ### Disconnect, TX topic:
  *   mosquitto_pub -t "application/2/device/de631ea3e5f58df3/command/tx" -h localhost -u "adailsilva" -P "@Hacker101" -m "{\"confirmed\":true,\"fPort\":101,\"data\":\"AA==\"}"
  *   
  *   ### Disconnect, DOWN topic:
  *   mosquitto_pub -t "application/2/device/de631ea3e5f58df3/command/down" -h localhost -u "adailsilva" -P "@Hacker101" -m "{\"confirmed\":true,\"fPort\":101,\"data\":\"AA==\"}"
  *   
  *   
  *   
  *   With Object, Unconfirmed Downlinks:
  *   
  *   ### Light up, TX topic:
  *   mosquitto_pub -t "application/2/device/de631ea3e5f58df3/command/tx" -h localhost -u "adailsilva" -P "@Hacker101" -m "{\"confirmed\":false,\"fPort\":101,\"data\":\"AQ==\",\"object\":{\"temperatureSensor\":{\"1\":25},\"humiditySensor\":{\"1\":32}}}"
  *   
  *   ### Light up, DOWN topic:
  *   mosquitto_pub -t "application/2/device/de631ea3e5f58df3/command/down" -h localhost -u "adailsilva" -P "@Hacker101" -m "{\"confirmed\":false,\"fPort\":101,\"data\":\"AQ==\",\"object\":{\"temperatureSensor\":{\"1\":25},\"humiditySensor\":{\"1\":32}}}"
  *   
  *   ### Disconnect, TX topic:
  *   mosquitto_pub -t "application/2/device/de631ea3e5f58df3/command/tx" -h localhost -u "adailsilva" -P "@Hacker101" -m "{\"confirmed\":false,\"fPort\":101,\"data\":\"AA==\",\"object\":{\"temperatureSensor\":{\"1\":25},\"humiditySensor\":{\"1\":32}}}"
  *   
  *   ### Disconnect, DOWN topic:
  *   mosquitto_pub -t "application/2/device/de631ea3e5f58df3/command/down" -h localhost -u "adailsilva" -P "@Hacker101" -m "{\"confirmed\":false,\"fPort\":101,\"data\":\"AA==\",\"object\":{\"temperatureSensor\":{\"1\":25},\"humiditySensor\":{\"1\":32}}}"
  *   
  *   
  *   No Object, Confirmed Downlinks:
  *   
  *   ### Light up, TX topic:
  *   mosquitto_pub -t "application/2/device/de631ea3e5f58df3/command/tx" -h localhost -u "adailsilva" -P "@Hacker101" -m "{\"confirmed\":true,\"fPort\":101,\"data\":\"AQ==\",\"object\":{\"temperatureSensor\":{\"1\":25},\"humiditySensor\":{\"1\":32}}}"
  *   
  *   ### Light up, DOWN topic:
  *   mosquitto_pub -t "application/2/device/de631ea3e5f58df3/command/down" -h localhost -u "adailsilva" -P "@Hacker101" -m "{\"confirmed\":true,\"fPort\":101,\"data\":\"AQ==\",\"object\":{\"temperatureSensor\":{\"1\":25},\"humiditySensor\":{\"1\":32}}}"
  *   
  *   ### Disconnect, TX topic:
  *   mosquitto_pub -t "application/2/device/de631ea3e5f58df3/command/tx" -h localhost -u "adailsilva" -P "@Hacker101" -m "{\"confirmed\":true,\"fPort\":101,\"data\":\"AA==\",\"object\":{\"temperatureSensor\":{\"1\":25},\"humiditySensor\":{\"1\":32}}}"
  *   
  *   ### Disconnect, DOWN topic:
  *   mosquitto_pub -t "application/2/device/de631ea3e5f58df3/command/down" -h localhost -u "adailsilva" -P "@Hacker101" -m "{\"confirmed\":true,\"fPort\":101,\"data\":\"AA==\",\"object\":{\"temperatureSensor\":{\"1\":25},\"humiditySensor\":{\"1\":32}}}"
  *   
  *   
  *   
  *   Limit 242 bytes:
  *   Em Hexadecimal:
  *   0001020304050607080900010203040506070809000102030405060708090001020304050607080900010203040506070809000102030405060708090001020304050607080900010203040506070809000102030405060708090001020304050607080900010203040506070809000102030405060708090001020304050607080900010203040506070809000102030405060708090001020304050607080900010203040506070809000102030405060708090001020304050607080900010203040506070809000102030405060708090001020304050607080900010203040506070809000102030405060708090001
  *   
  *   In Base64:
  *   AAECAwQFBgcICQABAgMEBQYHCAkAAQIDBAUGBwgJAAECAwQFBgcICQABAgMEBQYHCAkAAQIDBAUGBwgJAAECAwQFBgcICQABAgMEBQYHCAkAAQIDBAUGBwgJAAECAwQFBgcICQABAgMEBQYHCAkAAQIDBAUGBwgJAAECAwQFBgcICQABAgMEBQYHCAkAAQIDBAUGBwgJAAECAwQFBgcICQABAgMEBQYHCAkAAQIDBAUGBwgJAAECAwQFBgcICQABAgMEBQYHCAkAAQIDBAUGBwgJAAECAwQFBgcICQABAgMEBQYHCAkAAQIDBAUGBwgJAAE=
  *   
  */
