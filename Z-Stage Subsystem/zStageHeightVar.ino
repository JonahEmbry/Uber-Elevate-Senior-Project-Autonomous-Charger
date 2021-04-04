
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
double desHeight; // INPUT. we will measure an initial value for this depending on where we want the z stage to start
double initHeight; // INPUT. ""
double initS; // INPUT. ""


// Motor and linear actuator parameters
const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// for your motor
const double inchesPerRevolution = 0.0787402;   // lead screw  (2 mm converted to inches)
const double stepsPerInch = stepsPerRevolution/inchesPerRevolution;

// System geometric parameters
const double h0 = 33.375; // height of center of the top swivel hub mount from the ground
const double hb = 17; // height of the stepper motor (top of the long range frame)
const double xb = 10; // x measurement from the center of the front swivel hubs to the center of the stepper motor shaft
const double l = 10.5; // distance between swivel hub centers on each arm

// Extra small dimensions to make model more exact
const double d1 = 1; // vertical separation of the swivel hubs of paired arms at the mid joiner. positive means the swivel hub of the lower arm is higher than that of the higher arm
const double d2 = 3.5; // vertical distance from the top arm's swivel hub on the mid joiner to the spanning rod
const double d3 = 2.5; // distance from the pivot point of the stepper motor hinge to the centerline of the stepper motor
const double d4 = 1; // distance from the spanning rod arm to the lead screw centerline

// Compute steps required to go from intHeight to desHeight
double desTheta = asin((desHeight-(initHeight+d1))/(2*l));
const double bottomTheta = atan(-(h0-d2-hb)/xb);
const double effHypotenuse = sqrt(pow((h0-d2-hb),2) + pow(xb,2));

double desSprime = sqrt(pow(effHypotenuse,2) + pow(l,2) - 2*l*effHypotenuse*cos(desTheta+bottomTheta));
double desS = sqrt(pow(desSprime,2)-pow((d3-d4),2)); // accounts for the fact that the lead screw is not exactly coincident to the hinge point of the stepper motor mount nor to the spanning rod. In reality, its offset from the hinge point by 2.5, and from the spanning rod by 1
double deltaS = desS-initS;
int steps = floor(deltaS*stepsPerInch);

//initS = desS; // OUTPUT. Have the main controller cache this to give as an input for a later movement
//initHeight = desHeight; // OUTPUT. Have the main controller cache this to give as an input for a later movement

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

void loop()
{}
