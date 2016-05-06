/**
   Copyright (c) 2009 Andrew Rapp. All rights reserved.

   This file is part of XBee-Arduino.

   XBee-Arduino is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   XBee-Arduino is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with XBee-Arduino.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <XBee.h>
#include <Servo.h>
#include <Button.h>

/*
  This example is for Series 2 XBee
  Receives a ZB RX packet and sets a PWM value based on packet data.
  Error led is flashed if an unexpected packet is received
*/

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
// create reusable response objects for responses we expect to handle
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();

Servo servoA;
Servo servoB;

Button switch_servo = Button(5, BUTTON_PULLUP_INTERNAL);
Button switch_solenoide = Button(9, BUTTON_PULLUP_INTERNAL);

int actuator = 0;

void setup() {

  // start serial (Serial: 0 (RX) and 1 (TX). Used to receive (RX)
  // and transmit (TX) TTL serial data using the ATmega32U4 hardware
  // serial capability. Note that on the Leonardo, the Serial class
  // refers to USB (CDC) communication; for TTL serial on pins 0 and
  // 1, use the Serial1 class.

  Serial.begin(57600);

  Serial1.begin(57600);
  xbee.begin(Serial1);

  //Switch D5

  if (switch_servo.isPressed()) {

    actuator = 1;

    servoA.attach(A5);
    servoB.attach(A4);

    //Switch D9

  } else if (switch_solenoide.isPressed()) {

    actuator = 2;

    //Motor 1 direction - OUT1 & OUT2
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    //Motor 1 Speed
    pinMode(10, OUTPUT);

    //Motor 2 direction - OUT3 & OUT4
    pinMode(12, OUTPUT);
    pinMode(13, OUTPUT);
    //Motor 2 Speed
    pinMode(11, OUTPUT);

  } else {

    actuator = 3;

    //Motor 1 direction - OUT1 & OUT2
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    //Motor 1 Speed
    pinMode(10, OUTPUT);

    //Motor 2 direction - OUT3 & OUT4
    pinMode(12, OUTPUT);
    pinMode(13, OUTPUT);
    //Motor 2 Speed
    pinMode(11, OUTPUT);

  }

}


// continuously reads packets, looking for ZB Receive or Modem Status
void loop() {

  xbee.readPacket();

  if (xbee.getResponse().isAvailable()) {
    // got something

    if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
      // got a zb rx packet

      // now fill our zb rx class
      xbee.getResponse().getZBRxResponse(rx);

      // get value of the first byte in the data
      int data = rx.getData(0);


      // Actuator Servo
      if (actuator == 1 ) {

        data = map(data, 0, 255, 1, 180);

        servoA.write(data);

        servoB.write(data);



        // Actuator Solenoide - Switch D9
      } else if (actuator == 2) {


        data = map(data, 0, 255, 0, 255);

        digitalWrite(7, HIGH);
        digitalWrite(8, LOW);

        digitalWrite(12, HIGH);
        digitalWrite(13, LOW);

        if (data >= 125) {

          analogWrite(10, 255);
          analogWrite(11, 255);
          delay (8);
          analogWrite(10, 0);
          analogWrite(11, 0);

        } else if (data <= 124) {

          analogWrite(10, 0);
          analogWrite(11, 0);
        }



      } else if (actuator == 3) {

        data = map(data, 0, 255, 0, 255);


        digitalWrite(7, HIGH);
        digitalWrite(8, LOW);

        analogWrite(10, data);

        digitalWrite(12, HIGH);
        digitalWrite(13, LOW);

        analogWrite(11, data);

      }

    }
  }

}


