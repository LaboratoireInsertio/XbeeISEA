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

/*
This example is for Series 2 XBee
 Sends a ZB TX request with the value of analogRead(pin5) and checks 
 the status response for success
 */

// create the XBee object
#include "WProgram.h"
void setup();
void loop();
void sendPacket(XBeeAddress64 addr64, uint8_t val);
XBee xbee = XBee();

// create an array of Node objects with the SH + SL Address of the 
// receiving XBees
Node nodes[] = {
  Node(XBeeAddress64(0x0013a200, 0x40e66c13)),
  Node(XBeeAddress64(0x0013a200, 0x40e66dd7)),
  Node(XBeeAddress64(0x0013a200, 0x40e66c49)),
  Node(XBeeAddress64(0x0013a200, 0x40e66c18)),
  Node(XBeeAddress64(0x0013a200, 0x40e66c2f))
  };

  // for testing
  int pin5 = 0;

void setup() {
  // starts serial communication with computer
  Serial.begin(57600);

  // starts serial communication with xBee
  Serial1.begin(57600);
  xbee.setSerial(Serial1);
}

void loop() {
  // break down 10-bit reading into two bytes and place in payload
  pin5 = analogRead(5)/4;
  for (int i = 0; i < (sizeof(nodes)/sizeof(Node)); i++)
    nodes[i].setVal(pin5);
  //if (Serial.available()){
  //  val = int(Serial.read());
  //  Serial.println(val, DEC);
  //}
  //payload[0] = pin5 >> 8 & 0xff;
  //payload[1] = pin5 & 0xff;
  //payload[0] = pin5;

  for (int i = 0; i < (sizeof(nodes)/sizeof(Node)); i++)
    if (nodes[i].valueHasChanged())
      sendPacket(nodes[i].getAddress(), nodes[i].getVal());

  delay(50);
}

void sendPacket(XBeeAddress64 addr64, uint8_t val) {
  // Prepare the Zigbee Transmit Request API packet
  ZBTxRequest txRequest;
  // Set the destination address of the message
  txRequest.setAddress64(addr64);
  // Identifies the UART data frame for the host to correlate with a 
  // subsequent ACK (acknowledgment). If set to 0, no response is sent.
  txRequest.setFrameId(0);
  // Disable ACK (acknowledgement)
  txRequest.setOption(1);
  // THE data to be send
  uint8_t payload[] = {
    val
  };
  txRequest.setPayload(payload, sizeof(payload));
  // Send the message
  xbee.send(txRequest);
}


