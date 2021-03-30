/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo WristServo;  // create servo object to control a servo
Servo BaseServo;
Servo FingerServo;
// twelve servo objects can be created on most boards

int TarX = 5;
int TarY = 6;
int Length = 8;

int InY;
int InX;
 

void setup() {
  WristServo.attach(9);  // attaches the servo on pin 9 to the servo object
  FingerServo.attach(8);  // attaches the servo on pin 9 to the servo object
  BaseServo.attach(7);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  BaseServo.write(160);     //Test to determine where horizontal is
  delay(15);
  
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    WristServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    FingerServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    FingerServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    WristServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}
