/****************************************************************************** 
Test Code for Gantry System Stepper motors

******************************************************************************/
#include <Servo.h>
#include <math.h>

Servo ArmServo;  // create servo object to control a servo
Servo BaseServo;
Servo TipServo;


//Declare pin functions on Arduino
//#define stp 2
//#define dir 3
//#define MS1 4
//#define MS2 5
//#define MS3 6
//#define EN  7
//#define XHome 8

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
float FirstHorizontalMove;
float SecondHorizontalMove;
float FinalVerticalMove;
double StartB;
double StartA;
double StartT;
double HomeB = 0;
double HomeA = 164;
double HomeT = 80;
float theta1;
float theta2;

void setup() {
/*********************************************************************************************/
// Pins  
//  pinMode(stp, OUTPUT);
//  pinMode(dir, OUTPUT);
//  pinMode(MS1, OUTPUT);
//  pinMode(MS2, OUTPUT);
//  pinMode(MS3, OUTPUT);
//  pinMode(EN, OUTPUT);
//  pinMode(XHome, INPUT);

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
  TipServo.attach(10);  // attaches the servo on pin 9 to the servo object
  // BaseServo.attach(7);  // attaches the servo on pin 9 to the servo object

  //StartB = BaseServo.read();
  StartA = ArmServo.read();
  StartT = TipServo.read();

/********************************************************************************************/
// MATH
const float Pi = 3.14159; // To however many digits you want.

  //Known Variables
float L1=8;       //length of main arm (inches) 
float L2=2;       //length of charging arm  (inches)
float X=1;        //final port position in x direction (inches)
float Y=8;        //final port position in y direction (inches)
float Z=.5;        //final port position in Z direction (inches)
float orientation= 120;    //normal angle of the charge port relative to x axis (degrees)

//Unknown Variables
// theta1              % angle of main arm relative to x axis
theta2=orientation; // angle of charging arm relative to x axis
// d                   % gantry system movement in x direction
//x1                  % mid position in x direction
//y1                  % mid position in y direction

//Calculating the mid position
//This mid position is used to determine where the main arm needs to move
// to. From here the charging arm swings into position

//Finding x1 & y1:
float x1;
float y1;
if (orientation > 90){
  float a=L2*cos((180-theta2)*Pi/180);
  x1=X-a;
  float b=L2*sin((180-theta2)*Pi/180);
  y1=Y-b;
}
else{
  float a=L2*cos((theta2)*Pi/180);
  x1=X+a;
  float b=L2*sin((theta2)*Pi/180);
  y1=Y-b;
}
Serial.println(x1);
Serial.println(y1);

// Calculating theta1
                
theta1=-asin(y1/L1)*180/Pi+180;
Serial.println(theta1);

//Calculating the gantry movement distance required
 
D=(cos((180-theta1)*Pi/180)*L1)+x1;  // D is the initial location of the horizontal gantry
Serial.println(D);

// Plugging in Motion
float theta3=-asin(Y/L1)*180/Pi;       //angle between final main arm location and x axis
float d1=L1*cos((180-theta3)*Pi/180);
D1=X-d1;                        //final position of the base of the main arm
Serial.println(theta3);
Serial.println(d1);
Serial.println(D1);

float x2;
float y2;
if (orientation > 90) {
    float a1=cos((180-orientation)*Pi/180)*L2;
    float b1=sin((180-orientation)*Pi/180)*L2;
    x2=X-a1;             //final tip position in x
    y2=Y+b1;             //final tip position in y
}
else {
    float a1=cos(orientation*Pi/180)*L2;
    float b1=sin(orientation*Pi/180)*L2;
    x2=X-a1;             
    y2=Y+b1;
}
Serial.println(x2);
Serial.println(y2);



/***************************************************************************************/
// Math to stepper and servo movement
float StepsPerInch = 4800;
float HorizontalMaxDistance = 16; // 16 inches is the max travel on horizontal gantry to home
HomeMove = StepsPerInch * HorizontalMaxDistance;
FirstHorizontalMove = D * StepsPerInch;
SecondHorizontalMove = D1 * StepsPerInch;
FinalVerticalMove= Z * StepsPerInch;

Serial.println(FirstHorizontalMove);
Serial.println(SecondHorizontalMove);
}







//Main loop
void loop() {


/************************************************************************************/
// setup  
  while(Serial.available()){
      user_input = Serial.read(); //Read user input and trigger appropriate function
      //digitalWrite(EN, LOW); //Pull enable pin low to set FETs active and allow motor control
      digitalWrite(ENV, LOW); //Pull enable pin low to set FETs active and allow motor control
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
//  digitalWrite(stp, LOW);
//  digitalWrite(dir, LOW);
//  digitalWrite(MS1, LOW);
//  digitalWrite(MS2, LOW);
//  digitalWrite(MS3, LOW);
//  digitalWrite(EN, HIGH);

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


      //Position Arm in starting position
      if(StartA <= HomeA){
          for (int pos = StartA; pos <= HomeA; pos += 1) { // goes from 0 degrees to 180 degrees
            // in steps of 1 degree
            ArmServo.write(pos);              // tell servo to go to position in variable 'pos'
            delay(15);                       // waits 15ms for the servo to reach the position
            Serial.println(pos);
          } 
      }
      else {
          for (int pos = StartA; pos >= HomeA; pos -= 1) { // goes from 0 degrees to 180 degrees
            // in steps of 1 degree
            ArmServo.write(pos);              // tell servo to go to position in variable 'pos'
            delay(15);                       // waits 15ms for the servo to reach the position
            Serial.println(pos);
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
            Serial.println(pos);
          } 
      }
      else {
          for (int pos = StartT; pos >= HomeT; pos -= 1) { // goes from 0 degrees to 180 degrees
            // in steps of 1 degree
            TipServo.write(pos);              // tell servo to go to position in variable 'pos'
            delay(15);                       // waits 15ms for the servo to reach the position
            Serial.println(pos);
          } 
      }
  Serial.println("Tip is Positioned");
  delay(2000);










/************************************************************************************************/
// Gantry Going To Home Positions
  
  Serial.println("Gantry Going Home");  // Display that movement has started

  // Go to Home Positions
  
//for(x= 0; x<HomeMove; x++)  //Move Horizontal To Home Position
//  {
//    digitalWrite(dir, LOW); //Pull direction pin low to move "forward"
//    digitalWrite(stp,HIGH); //Trigger one step forward
//    delay(1);
//    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
//    delay(1);
//    if( (digitalRead(XHome) == LOW))  // Reset at home using this limit switch
//  {
//    Serial.println("HOME!");
//    delay(5000);
//    break;
//    //x=HomeMove; 
// 
//  }
//  }
//  

for(xx= 0; xx<HomeMove; xx++)  //Move Vertical To Home Position
  {
    digitalWrite(dirV, LOW); //Pull direction pin low to move "forward"
    digitalWrite(stpV,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stpV,LOW); //Pull step pin low so it can be triggered again
    delay(1);
    if( (digitalRead(YHome) == LOW))  // Reset at home using this limit switch
  {
    Serial.println("Vertical Gantry HOME!");
    delay(2000);
    break;
    //x=HomeMove; 
 
  }
  }
  














/**********************************************************************************/
//Vertical Gantry To Final Position

 for(yy= 0; yy<FinalVerticalMove; yy++)  // Move to the vertical final position
 {
   digitalWrite(dirV, HIGH); //Pull direction pin low to move "forward"
   digitalWrite(stpV,HIGH); //Trigger one step forward
   delay(1);
   digitalWrite(stpV,LOW); //Pull step pin low so it can be triggered again
   delay(1);  
  }
Serial.println("Vertical Final Position!"); 
delay(5000);














 /**********************************************************************************************/
 // Robotic arm & tip going to first position
 
 // Rotate Arm 
    Serial.println("Rotating Arm");
    delay(1000); 
    for (int pos = HomeA; pos >= (HomeA-theta1); pos -= 1) { // goes from 0 degrees to 180 degrees in steps of 1 degree
      ArmServo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
      Serial.println(pos);
      } 


// Rotate Tip
    Serial.println("Rotating Tip");
    delay(1000); 
    float TipPosition1=(HomeT+(theta2-(180-theta1)));
    Serial.println(TipPosition1);
    for (int pos = HomeT; pos <= TipPosition1; pos += 1) { // goes from 0 degrees to 180 degrees in steps of 1 degree
      TipServo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
      Serial.println(pos);
      } 







/*****************************************************************************************/
// Gantry going to first position


// Move To First Positions
 float NewFirstHorizontalMove=FirstHorizontalMove;
 Serial.println("Moving to first position");
 
// for(y= 0; y<NewFirstHorizontalMove; y++)  // Move to the initial position
// {
//   digitalWrite(dir, HIGH); //Pull direction pin low to move "forward"
//   digitalWrite(stp,HIGH); //Trigger one step forward
//   delay(1);
//   digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
//   delay(1);
//   if (y==32767)
//   {
//      y=0;
//      NewFirstHorizontalMove=NewFirstHorizontalMove-32767;
//   }  
//  }
//Serial.println("First Position!"); 
//delay(5000); 












 /**********************************************************************************************/
 // Robotic arm & tip going to Final position
 
 // Rotate Arm 
    Serial.println("Rotating Arm to Final Position");
    delay(1000); 
    //for (int pos = HomeA; pos >= (HomeA-theta1); pos -= 1) { // goes from 0 degrees to 180 degrees in steps of 1 degree
      ArmServo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
      Serial.println(pos);
      } 


// Rotate Tip
    Serial.println("Rotating Tip");
    delay(1000); 
    //float TipPosition2=(HomeT+(theta2-(180-theta1)));
    Serial.println(TipPosition2);
    for (int pos = TipPosition1; pos <= TipPosition2; pos += 1) { // goes from 0 degrees to 180 degrees in steps of 1 degree
      TipServo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
      Serial.println(pos);
      } 







// Move To Second Positions
 
// Serial.println("Moving to second position");
//   
//   
//   
//   
//float FinalHorizontalMove;   
//   if (D > D1)
//  {
//    FinalHorizontalMove=FirstHorizontalMove-SecondHorizontalMove;
//  }
//  else { 
//   FinalHorizontalMove=SecondHorizontalMove-FirstHorizontalMove;
//  }
// Serial.println(FinalHorizontalMove);
//   
// for(z= 0; z<FinalHorizontalMove; z++)  // Move to the final position
// {
//  if (D>D1)
//  {
//    digitalWrite(dir, LOW); //Pull direction pin low to move backwards
//  }
//  else { 
//   digitalWrite(dir, HIGH); //Pull direction pin low to move forwards
//  }
//   digitalWrite(stp,HIGH); //Trigger one step forward
//   delay(1);
//   digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
//   delay(1);
//   if (z==32767)
//   {
//      z=0;
//      FinalHorizontalMove=FinalHorizontalMove-32767;
//   }  
//  }
//Serial.println("Second Position!");  
  
  
  

}
