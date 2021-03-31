
/*
 Stepper Motor Control - one revolution

 This program drives a unipolar or bipolar stepper motor.
 The motor is attached to digital pins 8 - 11 of the Arduino.

 The motor should revolve one revolution in one direction, then
 one revolution in the other direction.


 Created 11 Mar. 2007
 Modified 30 Nov. 2009
 by Tom Igoe

 */

#include <Stepper.h>

// System inputs
double desHeight; // INPUT
double initHeight; // INPUT
double initS; // INPUT


// Motor and linear actuator parameters
const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// for your motor
const double inchesPerRevolution = 0.125;   // correct this guess later
const double stepsPerInch = stepsPerRevolution/inchesPerRevolution;

// System geometric parameters
const double h0 = 1; // meters, take measurement of system
const double hb = 0.3; // meters, take measurement of system
const double xb = 0.5; // meters, take measurement of system
const double l = 0.3; // meters, take measurement of system

// Compute steps required to go from intHeight to desHeight
double desTheta = asin((desHeight-initHeight)/(2*l));
const double bottomTheta = atan(-(h0-hb)/xb);
const double frameDiagonal = sqrt(pow((h0-hb),2) + pow(xb,2));

double desS = sqrt(pow(frameDiagonal,2) + pow(l,2) - 2*l*frameDiagonal*cos(desTheta+bottomTheta));
double deltaS = desS-initS;
int steps = floor(deltaS*stepsPerInch);

initS = desS; // OUTPUT. Have the main controller cache this to give as an input for a later movement
initHeight = desHeight; // OUTPUT. Have the main controller cache this to give as an input for a later movement

// initialize the stepper library on pins 8 through 11
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

void setup() {
  // set the speed at 60 rpm:
  myStepper.setSpeed(100);
  // initialize the serial port:
  Serial.begin(9600);
  Serial.println("moving to desired height");
  myStepper.step(steps);
}
