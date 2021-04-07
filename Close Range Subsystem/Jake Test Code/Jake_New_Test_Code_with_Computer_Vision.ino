/****************************************************************************** 
Test Code for Close Range System Using Computer Vision

******************************************************************************/
#include <Servo.h>
#include <math.h>

Servo ArmServo;  // create servo object to control a servo
Servo BaseServo; // create servo object to control a servo
Servo TipServo;  // create servo object to control a servo


//Declare pin functions on Arduino for the horizontal stepper motor
#define stp 2
#define dir 3
#define MS1 4
#define MS2 5
#define MS3 6
#define EN  7
#define XHome 8

//Declare pin functions on Arduino for the vertical stepper motor
#define stpV 2  // V stands for vertical gantry
#define dirV 3
#define MS1V 4
#define MS2V 5
#define MS3V 6
#define ENV  7
#define YHome 8

//Declare variables for functions
char user_input;
int x;
int y;
int z;
int xx;
int yy;
float D;
float D1;
int state;
float HomeMove;
float HorizontalAdjustmentMove;
double StartB;
double StartA;
double StartT;
double HomeB = 80;
double HomeA = 160;
double HomeT = 80;
float theta1;
float theta2;
float theta3;
int motor1pin1 = 22;
int motor1pin2 = 24;
float AprilTagX;
float AprilTagY;
float AprilTagZ;
float AprilTagOrientation;

void setup() {
/*********************************************************************************************/
// Pins  
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(XHome, INPUT);

  pinMode(stpV, OUTPUT);
  pinMode(dirV, OUTPUT);
  pinMode(MS1V, OUTPUT);
  pinMode(MS2V, OUTPUT);
  pinMode(MS3V, OUTPUT);
  pinMode(ENV, OUTPUT);
  pinMode(YHome, INPUT);

  
  resetBEDPins(); //Set step, direction, microstep and enable pins to default states
  Serial.begin(9600); //Open Serial connection for debugging
  Serial.println("Begin close range system operation");
  Serial.println();
  //Print function list for user selection
  Serial.println("Press 1 to start:");
  Serial.println();
  
  ArmServo.attach(9);  // attaches the servo on pin 9 to the servo object
  TipServo.attach(10);  // attaches the servo on pin 10 to the servo object
  BaseServo.attach(11);  // attaches the servo on pin 9 to the servo object

  StartB = BaseServo.read();
  StartA = ArmServo.read();
  StartT = TipServo.read();

  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);

/********************************************************************************************/
// MATH
const float Pi = 3.14159; // To however many digits you want.

  //Known Variables
float L1=6.3125;       //length of main arm (inches) 
float L2=3.15;       //length of charging arm  (inches)
float X=3;        //final port position in x direction (inches)
float A=X;        //distance between april tag and charge port
float Y=8;        //final port position in y direction (inches) (distance from april tag to OpenMV Camera)
float Z=.5;        //final port position in Z direction (inches)
float orientation= 45;    //normal angle of the charge port relative to x axis (degrees)
float B=2;        // distance between OpenMV camera and base of robotic arm

//Unknown Variables
// theta1              % angle of main arm relative to x axis
theta2=orientation; // angle of charging arm relative to x axis
// D                   % Additional offset distance for gantry
//x1                  % mid position in x direction
//y1                  % mid position in y direction

//Calculating the mid position
//This mid position is used to determine where the main arm needs to move
// to. From here the charging arm swings into position

//Finding x1 & y1:
float x1;
float y1;
if (orientation > 90){
  float a1=L2*cos((180-theta2)*Pi/180);
  x1=X-a1;
  float b1=L2*sin((180-theta2)*Pi/180);
  y1=b1;
}
else{
  float a1=L2*cos((theta2)*Pi/180);
  x1=A+a1;
  float b1=L2*sin((theta2)*Pi/180);
  y1=b1;
}
Serial.print("x1= ");
Serial.println(x1);
Serial.print("y1= ");
Serial.println(y1);

// Calculating theta1
                
theta1=-asin((Y-y1)/L1)*180/Pi+180;
Serial.print("theta1= ");
Serial.println(theta1);


// Calculating D (the gantry offset adjustment)
float D=x1-cos((180-theta1)*Pi/180)-B;
Serial.print("D= ");
Serial.println(D);




/***************************************************************************************/
// Math to stepper and servo movement
float StepsPerInch = 4800;
float MaxGantryDistance = 16; // 16 inches is the max travel on horizontal gantry to home
HomeMove = StepsPerInch * MaxGantryDistance;
HorizontalAdjustmentMove= StepsPerInch*D;
}







//Main loop
void loop() {


/************************************************************************************/
// setup  
  while(Serial.available()){
      user_input = Serial.read(); //Read user input and trigger appropriate function
      digitalWrite(EN, LOW); //Pull enable pin low to set FETs active and allow motor control
      //digitalWrite(ENV, LOW); //Pull enable pin low to set FETs active and allow motor control
      if (user_input =='1')
      {
         BeginMovement();
      }
      else
      {
        Serial.println("Awaiting next command");
      }
      resetBEDPins();
  }
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

  digitalWrite(stpV, LOW);
  digitalWrite(dirV, LOW);
  digitalWrite(MS1V, LOW);
  digitalWrite(MS2V, LOW);
  digitalWrite(MS3V, LOW);
  digitalWrite(ENV, HIGH);
}









/************************************************************************************/
// Robotic Arm Home Position


//Movement
void BeginMovement()



{
// Robotic Arm Home Position



        //Position Base Servo in starting position
      if(StartB <= HomeB){
          for (int pos = StartB; pos <= HomeB; pos += 1) { // goes from 0 degrees to 180 degrees
            // in steps of 1 degree
            BaseServo.write(pos);              // tell servo to go to position in variable 'pos'
            delay(15);                       // waits 15ms for the servo to reach the position
          } 
      }
      else {
          for (int pos = StartB; pos >= HomeB; pos -= 1) { // goes from 0 degrees to 180 degrees
            // in steps of 1 degree
            BaseServo.write(pos);              // tell servo to go to position in variable 'pos'
            delay(15);                       // waits 15ms for the servo to reach the position
          } 
      }
  Serial.println("Base Servo is Positioned");
  delay(1000);





      //Position Arm in starting position
      if(StartA <= HomeA){
          for (int pos = StartA; pos <= HomeA; pos += 1) { // goes from 0 degrees to 180 degrees
            // in steps of 1 degree
            ArmServo.write(pos);              // tell servo to go to position in variable 'pos'
            delay(15);                       // waits 15ms for the servo to reach the position
          } 
      }
      else {
          for (int pos = StartA; pos >= HomeA; pos -= 1) { // goes from 0 degrees to 180 degrees
            // in steps of 1 degree
            ArmServo.write(pos);              // tell servo to go to position in variable 'pos'
            delay(15);                       // waits 15ms for the servo to reach the position
          } 
      }
  Serial.println("Arm is Positioned");
  delay(2000);






        //Position Tip so it is parallel with Arm
      if(StartT <= HomeT){
          for (int pos = StartT; pos <= HomeT; pos += 1) { // goes from 0 degrees to 180 degrees
            // in steps of 1 degree
            TipServo.write(pos);              // tell servo to go to position in variable 'pos'
            delay(15);                       // waits 15ms for the servo to reach the position
            //Serial.println(pos);
          } 
      }
      else {
          for (int pos = StartT; pos >= HomeT; pos -= 1) { // goes from 0 degrees to 180 degrees
            // in steps of 1 degree
            TipServo.write(pos);              // tell servo to go to position in variable 'pos'
            delay(15);                       // waits 15ms for the servo to reach the position
            //Serial.println(pos);
          } 
      }
  Serial.println("Tip is Positioned");
  delay(2000);










/************************************************************************************************/
// Gantry Going To Home Positions
  
  Serial.println("Gantry Going Home");  // Display that movement has started

  // Go to Home Positions
  
for(x= 0; x<HomeMove; x++)  //Move Horizontal To Home Position
  {
    digitalWrite(dir, LOW); //Pull direction pin low to move towards home
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
    if( (digitalRead(XHome) == LOW))  // Reset at home using this limit switch
  {
    Serial.println("HOME!");
    delay(5000);
    break;
  }
  }
  


//Move Vertical To Home Position
for(xx= 0; xx<HomeMove; xx++)  
  {
    digitalWrite(dirV, LOW); //Pull direction pin low to move "upwards"
    digitalWrite(stpV,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stpV,LOW); //Pull step pin low so it can be triggered again
    delay(1);
    if( (digitalRead(YHome) == LOW))  // Reset at home using this limit switch
  {
    Serial.println("Vertical Gantry HOME!");
    delay(2000);
    break;
  }
  }
  

















/**********************************************************************************/
//Gantry Find April Tags

//Horizontal Movement To Allign With April Tag

  for(y=0; y<HomeMove; y++) //triggers horizontal gantry to go from home to the opposite end
  {
   digitalWrite(dir, HIGH); //Pull direction pin low to move "forward"
   digitalWrite(stp,HIGH); //Trigger one step forward
   delay(1);
   digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
   delay(1);
   if( AprilTagX == 0)  // Horizontal Gantry is now alligned with AprilTag in x axis
  {
    Serial.println("Horizontal Gantry Alligned with April Tag");
    delay(2000);
    break;
  }
  }


//Vertical Movement To Allign With April Tag

  for(yy=0; yy<HomeMove; yy++) //triggers horizontal gantry to go from home to the opposite end
  {
   digitalWrite(dir, HIGH); //Pull direction pin low to move down
   digitalWrite(stp,HIGH); //Trigger one step forward
   delay(1);
   digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
   delay(1);
   if( AprilTagY == 0)  // Horizontal Gantry is now alligned with AprilTag in x axis
  {
    Serial.println("Vertical Gantry Alligned with April Tag");
    delay(2000);
    break;
  }
  }
















/**********************************************************************************/
//Horizontal Movement Gantry Adjustment Offset Psotion

   Serial.println("Horizontal Gantry Moving to offset adjustment position");
   if (D>=0)
   {
   for(z=0; z<HorizontalAdjustmentMove; z++) //triggers horizontal gantry to go from april tag to the adjustment position
  {
   digitalWrite(dir, HIGH); //Pull direction pin low to move "forward"
   digitalWrite(stp,HIGH); //Trigger one step forward
   delay(1);
   digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
   delay(1);
  }
   }
   else
   {
   for(z=0; z<HorizontalAdjustmentMove; z++) //triggers horizontal gantry to go from april tag to the adjustment position
  {
   digitalWrite(dir, LOW); //Pull direction pin low to move backwards
   digitalWrite(stp,HIGH); //Trigger one step forward
   delay(1);
   digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
   delay(1);
  }
   }

   















/**********************************************************************************************/
 // Robotic arm & tip going to final position
 
 // Rotate Arm 
    Serial.println("Rotating Arm");
    delay(1000); 
    for (int pos = HomeA; pos >= (HomeA-theta1); pos -= 1) { 
      ArmServo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
      //Serial.println(pos);
      } 


// Rotate Tip
    Serial.println("Rotating Tip");
    delay(1000); 
    if (theta2 >= (180-theta1))  //depends on if the tip is rotating clockwise or counterclockwise compared to theta1
    {
    float TipPosition1=HomeT+(theta2-(180-theta1));
    Serial.println(TipPosition1);
    for (int pos = HomeT; pos <= TipPosition1; pos += 1) { 
      TipServo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
      //Serial.println(pos);
      } 
    }
    else
    {
    float TipPosition1=HomeT-((180-theta1)-theta2);
    Serial.println(TipPosition1);
    for (int pos = HomeT; pos >= TipPosition1; pos -= 1) { 
      TipServo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
     // Serial.println(pos);
      } 
    }





/**************************************************************************************/
//Plugging In Mechanism using the DC motor

  // Pushes charging tip out
  delay(2000);
  Serial.println("Plugging in!");
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);
  delay(7000);
  Serial.println("Plugged In!");
  Serial.println();
  Serial.println();
  Serial.println();
  delay(3000);
  Serial.println("Aircraft Charging");



  // Pauses for 10 seconds
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, LOW);
  delay(10000);



  //Pulls charging tip in  
  Serial.println("Unplugging!");  
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);
  delay(7000);
  
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, LOW);
  delay(10000);
}

  
