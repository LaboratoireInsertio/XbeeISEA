/**
 * Copyright (c) 2009 Andrew Rapp. All rights reserved.
 *
 * This file is part of XBee-Arduino.
 *
 * XBee-Arduino is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * XBee-Arduino is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with XBee-Arduino.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <XBee.h>
#include <Node.h>
#include <Slip.h>

/*
This example is for Series 2 XBee
 Sends a ZB TX request with the value of analogRead(pin5) and checks 
 the status response for success
 */

// create the XBee object
#include "WProgram.h"
void setup();
void loop();
void onReceive();
void sendPacket(XBeeAddress64 addr64, uint8_t val);
XBee xbee = XBee();

// START Slip AND LINK IT TO THE onReceive FUNCTION
// THAT WILL BE CALLED WHEN A MESSAGE IS RECEIVED
Slip slip = Slip(Serial, onReceive); 

// create an array of Node objects with the SH + SL Address of the 
// receiving XBees and the assigned notes
Node nodes[] = { 
  Node(XBeeAddress64(0x0013a200, 0x40e66c13), 72),  // 72 - C4
  Node(XBeeAddress64(0x0013a200, 0x40e66dd7), 73),  // 73 - C#4
  Node(XBeeAddress64(0x0013a200, 0x40e66c49), 74),  // 74 - D4
  Node(XBeeAddress64(0x0013a200, 0x40e66c18), 75),  // 75 - D#4
  Node(XBeeAddress64(0x0013a200, 0x40e66c2f), 67)   // 76 - E4
  };

  // FOR TESTING
  //int pin5 = 0;

  void setup() {
    // starts serial communication with computer
    Serial.begin(57600);

    // starts serial communication with xBee
    Serial1.begin(57600);
    xbee.setSerial(Serial1);
  }

void loop() {
  // break down 10-bit reading into two bytes and place in payload
  // USING PIN 5 FOR TESTING, UPDATE WITH VALUES FROM LIVE
  //pin5 = analogRead(5)/4;

  // update() MUST BE CALLED EVERY LOOP
  slip.update();
}

// THIS FUNCTION IS CALLED WHEN A SLIP MESSAGE IS RECEIVED
void onReceive() {

  // GET THE LENGTH IN BYTES OF THE RECEIVED DATA
  int length = slip.length();

  // HERE WE CAN CHECK IF WE RECEIVED THE EXPECTED NUMBER OF BYTES
  if ( length == 2 ) {
    // WE CAN GET INDIVIUAL BYTES WITH get()
    byte destinationXbee = slip.get(0); // first byte
    byte dataToSend = slip.get(1); //second byte
    dataToSend = map(dataToSend,1,127,0,255);

    for (int i = 0; i < (sizeof(nodes)/sizeof(Node)); i++)
      if (nodes[i].getNote() == destinationXbee)
        nodes[i].setVal(dataToSend);

    // sends messages to the nodes only if the values have changed
    for (int i = 0; i < (sizeof(nodes)/sizeof(Node)); i++)
      if (nodes[i].valueHasChanged())
        sendPacket(nodes[i].getAddress(), nodes[i].getVal());
  }
}


void sendPacket(XBeeAddress64 addr64, uint8_t val) {
  // Prepare the Zigbee Transmit Request API packet
  ZBTxRequest txRequest;
  // Set the destination address of the message
  txRequest.setAddress64(addr64);
  // THE data to be send
  uint8_t payload[] = {
    val
  };
  txRequest.setPayload(payload, sizeof(payload));
  // Identifies the UART data frame for the host to correlate with a 
  // subsequent ACK (acknowledgment). If set to 0, no response is sent.
  txRequest.setFrameId(0);
  // Disable ACK (acknowledgement)
  txRequest.setOption(1);
  // Send the message
  xbee.send(txRequest);
}


