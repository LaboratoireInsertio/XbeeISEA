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
void sendAtCommand();
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

  // FOR TESTING
  int pin5 = 0;

// ------------- NODE DISCOVERY TEST ------------- //

// node discovery command
uint8_t ndCmd[] = {
  'N','D'
};
uint8_t shCmd[] = {
  'S','H'
};
AtCommandRequest atRequest = AtCommandRequest(ndCmd);
AtCommandResponse atResponse = AtCommandResponse();

// ------------- ---- --------- ---- ------------- //

void setup() {
  // starts serial communication with computer
  Serial.begin(57600);

  // starts serial communication with xBee
  Serial1.begin(57600);
  xbee.setSerial(Serial1);

  // ------------- NODE DISCOVERY TEST ------------- //

  // startup delay to wait for XBee radio to initialize.
  // you may need to increase this value if you are not getting a 
  // response
  delay(5000);

  sendAtCommand();

  // ------------- ---- --------- ---- ------------- //
}

void loop() {
  // break down 10-bit reading into two bytes and place in payload
  // USING PIN 5 FOR TESTING, UPDATE WITH VALUES FROM LIVE
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

  // sends messages to the nodes only if the values have changed
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

void sendAtCommand() {
  Serial.println("Sending command to the XBee");

  // send the command
  xbee.send(atRequest);

  // wait up to 5 seconds for the status response
  if (xbee.readPacket(5000)) {
    // got a response!

    // should be an AT command response
    if (xbee.getResponse().getApiId() == AT_COMMAND_RESPONSE) {
      xbee.getResponse().getAtCommandResponse(atResponse);

      if (atResponse.isOk()) {
        Serial.print("Command [");
        Serial.print(atResponse.getCommand()[0]);
        Serial.print(atResponse.getCommand()[1]);
        Serial.println("] was successful!");

        if (atResponse.getValueLength() > 0) {
          Serial.print("Command value length is ");
          Serial.println(atResponse.getValueLength(), DEC);

          Serial.print("Command value: ");

          for (int i = 0; i < atResponse.getValueLength(); i++) {
            Serial.print(atResponse.getValue()[i], HEX);
            Serial.print(" ");
          }

          Serial.println("");
        }
      } 
      else {
        Serial.print("Command return error code: ");
        Serial.println(atResponse.getStatus(), HEX);
      }
    } 
    else {
      Serial.print("Expected AT response but got ");
      Serial.print(xbee.getResponse().getApiId(), HEX);
    }
  } 
  else {
    // at command failed
    if (xbee.getResponse().isError()) {
      Serial.print("Error reading packet.  Error code: ");  
      Serial.println(xbee.getResponse().getErrorCode());
    } 
    else {
      Serial.print("No response from radio");
    }
  }
}


