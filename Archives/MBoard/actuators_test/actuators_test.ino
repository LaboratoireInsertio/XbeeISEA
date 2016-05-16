#include <Button.h>

#include <Servo.h>



Servo myservo;

int pos = 0;

Button switch_servo = Button(5, BUTTON_PULLUP_INTERNAL);
Button switch_solenoide = Button(9, BUTTON_PULLUP_INTERNAL);

int speed_ = 0;    // Motor speed_
int increment = 5;     // speed_ increment


void setup() {



  myservo.attach(A5);

   pinMode(14, OUTPUT);
    
  //Motor 1 direction - OUT1 & OUT2
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  //Motor 1 speed_
  pinMode(10, OUTPUT);

  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);

  //Motor 2 direction - OUT3 & OUT4
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  //Motor 2 speed_
  pinMode(11, OUTPUT);

  digitalWrite(12, HIGH);
  digitalWrite(13, LOW);

}

// continuously reads packets, looking for ZB Receive or Modem Status
void loop() {

  // Servo
  if (switch_servo.isPressed()) {

    digitalWrite(14, HIGH);
    delay (1000);
    digitalWrite(14, LOW);
    delay (1000);


     analogWrite(11, 0);
    
    for (pos = 0; pos < 180; pos += 1) // goes from 0 degrees to 180 degrees
    { // in steps of 1 degree
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
    for (pos = 180; pos >= 1; pos -= 1) // goes from 180 degrees to 0 degrees
    {
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);
    }

    // MOTOR 1 - Solenoide
  } else if (switch_solenoide.isPressed()) {

    digitalWrite(14, HIGH);
      delay (500);
      digitalWrite(14, LOW);
      delay (500); 


  
     //analogWrite(11, 0);
     
    analogWrite(10, 255);
    delay (35);
    analogWrite(10, 0);
    delay (35);

         
    analogWrite(11, 255);
    delay (50);
    analogWrite(11, 0);
    delay (50);

    // MOTOR 2 - DC
    
  } else {

    digitalWrite(14, HIGH);

    /* 
    myservo.write(pos); 
    //analogWrite(11, speed_);
    analogWrite(10, 0);
    */

    speed_ = speed_ + increment;  // increment speed_ for next loop iteration

    if (speed_ <= 0 || speed_ >= 125)    // reverse the direction of the fading
    {
      increment = -increment;
    }
    
    speed_ = constrain(speed_, 0, 255);
    analogWrite(11, speed_);

    delay (100);
  }


}



