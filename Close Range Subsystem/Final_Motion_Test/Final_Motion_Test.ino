// Written by Jonah Embry
// Updated: 3/26/2021
// The goal of this code is to prove that linear motion can be achieved using the two parallel rotational DOF (servos) and the translational DOF (stepper)

#include <Servo.h>
#include <math.h>

// Servos
Servo ArmServo;  // create servo object to control a servo
Servo TipServo;  //

// Stepper
#define stp 2
#define dir 3
#define MS1 4
#define MS2 5
#define MS3 6
#define EN  7

int InitTipPOS = 0; // Initital Arm angle
int InitArmPOS = 0; // Intitial Arm angle
int FinTipPOS = 0;  // Final Tip angle
int FinArmPOS = 0;  // Final Arm angle

int InitXPOS = 0;   // Initial X position
int FinXPOS = 0;    //  Final X position

int steps = 0;      // Number of steps the system takes in the plugging in motion
int timer = 10;    // Number of milliseconds to wait between motions
void setup() {

  Serial.begin(9600);

  // Servo
  ArmServo.attach(8);  // attaches the servo on pin 8 to the servo object
  TipServo.attach(9);  // attaches the servo on pin 9 to the servo object

  // Stepper
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);
  pinMode(EN, OUTPUT);
  resetBEDPins(); //Set step, direction, microstep and enable pins to default states
  
}

void loop() {
  // Assume that the system is positioned so that the charger is aligned with the port. Here is the code for the linear motion

  for(int i = 0; i <= steps; i++){
 
    //Tip Rotation Step
    TipServo.write(((FinTipPOS - InitTipPOS)/steps)*i);             
    delay(timer);
    //Arm Rotation Step
    ArmServo.write(((FinArmPOS - InitArmPOS)/steps)*i);
    delay(timer);
    //Base Movement Step
    for(int ii = 0; ii < ((FinXPOS-InitXPOS)/steps)*i; ii++){
      digitalWrite(stp,HIGH); //Trigger one step forward
      delay(1);
      digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
      delay(1);
    }
    delay(timer);
  }

/*
    The idea behind this code is that I've made the Arduino version of the linspace function from
    Matlab. We choose some step size in the variable 'steps'. We then divide the difference in 
    position (angular for servos and locational for the stepper) by this number of steps. The
    bigger for loop moves the system through these steps, tracking which step it is on using the
    variable 'i' from the loop. This method normalizes the differences into the same number of 
    steps and ensures that the movement keeps the charger lined up with the port.

*/

}


//Reset Big Easy Driver pins to default states
void resetBEDPins()
{
  digitalWrite(stp, LOW);
  digitalWrite(dir, LOW);
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(MS3, LOW);
  digitalWrite(EN, HIGH);
}

//  // Example of how to move steppers
//
//  Serial.println("Moving forward at default step mode.");
//  digitalWrite(dir, LOW); //Pull direction pin low to move "forward"
//  for(x= 0; x<1000; x++)  //Loop the forward stepping enough times for motion to be visible
//  {
//    digitalWrite(stp,HIGH); //Trigger one step forward
//    delay(1);
//    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
//    delay(1);
//  }
//  Serial.println("Enter new option");
//  Serial.println();
//
//
//// Example of how to move servo
//          
//  for (int pos = StartB; pos <= HomeB; pos += 1) { // goes from 0 degrees to 180 degrees
//    // in steps of 1 degree
//    BaseServo.write(pos);              // tell servo to go to position in variable 'pos'
//    delay(15);                       // waits 15ms for the servo to reach the position
//    Serial.println(pos);
//  } 
