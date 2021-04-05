// Written by Jonah Embry
// Updated 4/5/2021
// This the communications code for the Z-Stage of the Uber Elevate Senior Capstone Design Project: Autonomous Charging Vehicle
// This code waits for confirmation of position from the Long Range System, then elevates to the appropriate height, then sends
// confirmation back to the Long Range System
// A status LED is included to display states and assist in trouble shooting
// REMEMBER THAT THE ARDUINOS NEED TO SHARE A COMMON GROUND OR IT WILL NOT WORK

// Include the required Wire library for I2C<br>
#include <Wire.h>
// Stepper Control
#include <Stepper.h>

// LEDs
int LED = 7;

// Communications
char LRSDone = '0';
char ZSDone = '0';
char Safety = '0';
 

// ************************************************** Stepper Stuff *******************************************
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
  
  Serial.begin(9600);     //Serial Line for Trouble Shooting
  pinMode (LED, OUTPUT);  // Define the LED pin as Output

  //Communication Stuff
  Wire.begin(8);          // Start the I2C Bus as Slave on address 8
  Wire.onReceive(receiveEvent); // Attach a function to trigger when something is received.
  Wire.onRequest(requestEvent); // register event -> When LRS asks for update, need to give status of Z-Stage (0 = not ready, 1 = ready)

  //Stepper Stuff
  // set the speed at 60 rpm:
  myStepper.setSpeed(100);
  // initialize the serial port:
  Serial.begin(9600);
}


void receiveEvent(int bytes) {
  LRSDone = Wire.read();          // If LRS sends message, it likely means that it is in position (This may be changed in future versions for E-stops)
  Serial.print("LRS is in position ");
  Serial.println(LRSDone);
}

void requestEvent() {             //Used code from here: https://www.arduino.cc/en/Tutorial/LibraryExamples/MasterReader
  Wire.write(ZSDone);            // respond with message of 1 bytes; Every time LRS Requests an update, it responds with the status of ZS. This should automatically change one in the void loop function once in position
  Serial.print("LRS wants update: ");
  Serial.println(ZSDone);
  // as expected by master
}

void loop() {
  
  Serial.println("Waiting for LRS");
  if (LRSDone == '1' && Safety == '0') {        //Char received is 1, LRS is in position and it is time to elevate 
    digitalWrite(LED, HIGH);   //Blink LED Indicator
    delay(500);
    digitalWrite(LED, LOW);
    delay(500);
    

    //Z-Stage Driving Code Goes Here:

    Serial.println("Moving to desired height");
    digitalWrite(LED, HIGH);
    delay(5000);
    digitalWrite(LED, LOW);
    //myStepper.step(steps);




    //Z-Stage is in position
    ZSDone = '1';
    Serial.println("ZDone = 1 ***************************");
    LRSDone = '0';
    Safety = '1';
    
  }
}
