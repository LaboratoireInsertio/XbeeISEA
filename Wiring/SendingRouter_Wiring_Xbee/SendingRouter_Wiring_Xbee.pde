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

/*
This example is for Series 2 XBee
 Sends a ZB TX request with the value of analogRead(pin5) and checks 
 the status response for success
 */

// create the XBee object
XBee xbee = XBee();

//uint8_t payload[] = { 
//  0
//};

// SH + SL Address of receiving XBee
XBeeAddress64 addr64_n0 = XBeeAddress64(0x0013a200, 0x40e66c13);
XBeeAddress64 addr64_n1 = XBeeAddress64(0x0013a200, 0x40e66dd7);
XBeeAddress64 addr64_n2 = XBeeAddress64(0x0013a200, 0x40e66c49);
XBeeAddress64 addr64_n3 = XBeeAddress64(0x0013a200, 0x40e66c18);
XBeeAddress64 addr64_n4 = XBeeAddress64(0x0013a200, 0x40e66c2f);

// for broadcasting to all nodes in the network
XBeeAddress64 addr64_broad = XBeeAddress64(0x00000000, 0x0000ffff);

//ZBTxRequest zbTx = ZBTxRequest(addr64_n0, payload, sizeof(payload));
//ZBTxStatusResponse txStatus = ZBTxStatusResponse();

int pin5 = 0;
int lastPin5 = 0;
//int val;

int statusLed = 15;
int errorLed = 15;


void setup() {
  pinMode(statusLed, OUTPUT);
  pinMode(errorLed, OUTPUT);

  Serial.begin(57600);

  Serial1.begin(57600);
  xbee.setSerial(Serial1);
}

void loop() {
  xbee.readPacket();

  // break down 10-bit reading into two bytes and place in payload
  lastPin5 = pin5;
  pin5 = analogRead(5)/4;
  //if (Serial.available()){
  //  val = int(Serial.read());
  //  Serial.println(val, DEC);
  //}
  //payload[0] = pin5 >> 8 & 0xff;
  //payload[1] = pin5 & 0xff;
  //payload[0] = pin5;
  
  Serial.print(abs(pin5 - lastPin5),DEC);
  Serial.print(" ");
  Serial.println();


  // send only when something changes
  if(abs(pin5 - lastPin5) > 2){
    sendPacket(addr64_n0);
    sendPacket(addr64_n1);
    sendPacket(addr64_n2);
    sendPacket(addr64_n3);
    sendPacket(addr64_n4);
    //sendPacket(addr64_broad);
  }




  delay(50);
}

void sendPacket(XBeeAddress64 addr64) {
  // Prepare the Zigbee Transmit Request API packet
  ZBTxRequest txRequest;
  txRequest.setAddress64(addr64);
  uint8_t payload1[] = {
    0
  };
  payload1[0] = pin5;
  txRequest.setPayload(payload1, sizeof(payload1));
  // And send it
  xbee.send(txRequest);

  /*
  // after sending a tx request, we expect a status response
   // wait up to half second for the status response
   if (xbee.readPacket(500)) {
   // got a response!
   
   // should be a znet tx status            	
   if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
   xbee.getResponse().getZBTxStatusResponse(txStatus);
   
   // get the delivery status, the fifth byte
   if (txStatus.getDeliveryStatus() == SUCCESS) {
   // success.  time to celebrate
   flashLed(statusLed, 5, 50);
   Serial.println(":)");
   } 
   else {
   // the remote XBee did not receive our packet. is it powered on?
   flashLed(errorLed, 3, 500);
   //Serial.println(":(");
   }
   }
   } 
   else if (xbee.getResponse().isError()) {
   //nss.print("Error reading packet.  Error code: ");  
   //nss.println(xbee.getResponse().getErrorCode());
   } 
   else {
   // local XBee did not provide a timely TX Status Response -- should not happen
   flashLed(errorLed, 2, 50);
   Serial.println(":(");
   }
   */
}

