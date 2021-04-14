  
// Written by Jonah Embry
// Last Updated: 3/26/2021
// This is the overall code for the LRS Communication Protocols
// In this system, the LRS Mega fuctions as the Master: Z-stage (Device 8) and (CRS Device 9) are slaves
// This version of the code is meant purely to plug the charger in. We can worry about uplugging it later
// Include the required Wire library for I2C<br>
// MAKE SURE ALL BOARDS SHARE COMMON GROUND

#include <RedBot.h>
#include <Wire.h>



//LED pins
#define errorPin 38 //RED
#define successPin 36 //Green
#define leftPin 30 //BLUE
#define centerPin 32 //BLUE
#define rightPin 34 //BLUE

//pins for Ultrasonic sensors
#define trigPin1 13
#define echoPin1 12
#define trigPin2 4
#define echoPin2 5

//pins for Communication
#define Zout 41
#define Zin 39
#define CRSout 46
#define CRSin 48


//motor right
#define motor_r 44
//motor left
#define motor_l 45

#define switchPin 51  //Bump Switch



//Redbot sensors
RedBotSensor left_sen = RedBotSensor(A0);  
RedBotSensor center_sen = RedBotSensor(A1);
RedBotSensor right_sen = RedBotSensor(A2);  
// constants that are used in the code. lineStandard is the level to detect
// if the sensor is on the line or not. If the sensor value is greater than this
// the sensor is above a DARK line.



// Various Constants
int lineStandard = 910;
const int turnSlowSpeed = 60;
const int turnFastSpeed = 75;
const int goSpeed = 70;
const int stopSpeed = 0;
const int StopDist = 50;
int flag = 0;
int bigFlag = 0;

//servo brakes
const int Brakes1 = 25;
const int Brakes2 = 27;
const int brakeTight = 1700;
const int brakeLoose = 1100;
const int UPDATE_TIME = 20;


int LED = 8;      //LRS Indicator LED
char ZDone = '0';    //Z-stage flag variable: if 0, z-stage is not elevated; if 1, z-stage is elevated
char CRSDone = '0';  //CRS flag variabe: if 0, CRS is not plugged in; if 1, CRS is plugged in

char user_input;  //Variable for user input for the start

void setup() {
  //outputs for line following sensors
  pinMode(motor_r, OUTPUT);
  pinMode(motor_l, OUTPUT);

  //variables for ultrasonics
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  //variables for communicatino
  pinMode(Zout, OUTPUT);
  pinMode(Zin, INPUT);
  pinMode(CRSout, OUTPUT);
  pinMode(CRSin, INPUT);

  //variables for LEDs
  pinMode(errorPin,OUTPUT);
  pinMode(successPin,OUTPUT);
  pinMode(leftPin,OUTPUT);
  pinMode(centerPin,OUTPUT);
  pinMode(rightPin,OUTPUT);

  //Limit Switch
  pinMode(switchPin,INPUT);
  Serial.begin(9600);  // Serial Line for Trouble Shooting
  pinMode(LED, OUTPUT); // LRS Indicator LED
    
  // Communications Stuff
  Wire.begin();           // Start the I2C Bus as Master
  
}
void loop() {
     //Start program
if(bigFlag==0){    
      //Start LRS Movement Function       
//LRS Driving Code Goes Here:
if(flag==0){
  Brakes(Brakes1,brakeLoose, UPDATE_TIME);
  Brakes(Brakes2,brakeLoose, UPDATE_TIME);
  flag = 1;
}


  
  //read IR sensors
  int Left = left_sen.read();
  int Center = center_sen.read(); 
  int Right = right_sen.read(); 
  
  //ultrasonic right
  long duration1, distance1;  
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = (duration1/2) / 29.1; //centimeters


  //ultrasonic left
  long duration2, distance2;  
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = (duration2/2) / 29.1; //centimeters

  //IR plus Ultrasonic sensor readings

  Serial.print("\t");
  Serial.print(distance1);
  Serial.print(" cm");
  Serial.print("\t");
  Serial.print(distance2);
  Serial.print(" cm");
  Serial.print("\t");
  Serial.print(Left);
  Serial.print("\t");  
  Serial.print(Center);
  Serial.print("\t");
  Serial.print(Right);
  Serial.print("\t");
  Serial.print(bigFlag);
  Serial.println();
  
 

//data_sheet
bool qtil = Left<lineStandard;
bool qtim = Center<lineStandard;
bool qtir = Right<lineStandard;

if (distance1<StopDist || distance2<StopDist){
  Stop();
  Brakes(Brakes1,brakeTight, UPDATE_TIME);
  Brakes(Brakes2,brakeTight, UPDATE_TIME);
  delay(4000);
}
else if(!qtil && qtim && !qtir){ //On the middle
  forward();
}
else if(qtil && !qtim && !qtir){ //On the left
  right();
}
else if(!qtil && !qtim && qtir){ //On the right
  left();
}
else if(qtil && qtim && !qtir){ //On the left and center
  right();
}
else if(!qtil && qtim && qtir){ //On the right and center
  left();
}
else if(!qtil && !qtim && !qtir){ //All black
  Stop();
}
else { //All white
  bigFlag=1;
  Success();
}
}
if (bigFlag==1){
//      LRS to Z-Stage: Start elevation
      Wire.beginTransmission(8); // transmit to device #8 (Z-Stage)
      Wire.write('1');              // sends 1 
      Wire.endTransmission();    // stop transmitting
      Serial.print("HIPPOOOOOOOOOO");


      //Z-stage to LRS: Finished elevation
          //Used code from here: https://www.arduino.cc/en/Tutorial/LibraryExamples/MasterReader
  
      while (ZDone == '0') {    // waiting for Z-Stage to confirm elevation
        delay(20000);
        Serial.println("Waiting for Z-Confirmation ");
        Wire.requestFrom(8, 1);                       // request 1 bytes from slave device #8 (Z-stage)
        ZDone = Wire.read();          // receive a byte as character
        Serial.print(ZDone);         // print the character
        //Blink LRS LED to display waiting
        Serial.println();
        delay(1000);                  // Checks if ZDone has been triggered every second
      }

        //At this point, Z-Stage should be in position


      //LRS to CRS: Plug in the charger
      Wire.beginTransmission(9); // transmit to device #9 (Close Range System)
      Wire.write('1');              // sends 1 
      Wire.endTransmission();    // stop transmitting


      //CRS to LRS: Charger Plugged in
    
      while (CRSDone == '0') {    // Waiting for CRS to confirm plug in
        Serial.println("Waiting for CRS Confirmation ");
        Wire.requestFrom(9, 1);                       // request 1 bytes from slave device #9 (CRS)
        CRSDone = Wire.read();          // receive a byte as character
        Serial.print(CRSDone);         // print the character
        //Blink LRS LED to display waiting
        Serial.println();
        delay(1000);                  // Checks if CRSDone has been triggered every second
      }

      //At this point, the CRS should be plugged in

}
}
    

void right() {
  digitalWrite(successPin,HIGH);
  digitalWrite(errorPin,LOW);
  digitalWrite(leftPin,LOW);
  digitalWrite(centerPin,LOW);
  digitalWrite(rightPin,HIGH);
  Brakes(Brakes1,brakeLoose, UPDATE_TIME);
  Brakes(Brakes2,brakeLoose, UPDATE_TIME);
  delay(100);
  delay(100);
  analogWrite(motor_r,turnFastSpeed);
  analogWrite(motor_l,turnSlowSpeed);
  
}
void Stop () {
  digitalWrite(successPin,LOW);
  digitalWrite(errorPin,HIGH);
  digitalWrite(leftPin,LOW);
  digitalWrite(centerPin,LOW);
  digitalWrite(rightPin,LOW);
  analogWrite(motor_r, stopSpeed);
  analogWrite(motor_l, stopSpeed);
  delay(100);
  Brakes(Brakes1,brakeTight, UPDATE_TIME);
  Brakes(Brakes2,brakeTight, UPDATE_TIME);
  delay(100);
}
void left() {
  digitalWrite(successPin,HIGH);
  digitalWrite(errorPin,LOW);
  digitalWrite(leftPin,HIGH);
  digitalWrite(centerPin,LOW);
  digitalWrite(rightPin,LOW);
  delay(100);
  Brakes(Brakes1,brakeLoose, UPDATE_TIME);
  Brakes(Brakes2,brakeLoose, UPDATE_TIME);
  delay(100);
  analogWrite(motor_r, turnSlowSpeed);
  analogWrite(motor_l, turnFastSpeed);
}


void forward() {
  digitalWrite(successPin,HIGH);
  digitalWrite(errorPin,LOW);
  digitalWrite(leftPin,LOW);
  digitalWrite(centerPin,HIGH);
  digitalWrite(rightPin,LOW);
  delay(100);
  Brakes(Brakes1,brakeLoose, UPDATE_TIME);
  Brakes(Brakes2,brakeLoose, UPDATE_TIME);
  delay(100);
  analogWrite(motor_r, goSpeed);
  analogWrite(motor_l, goSpeed);
}
void Success() {
  digitalWrite(successPin,HIGH);
  digitalWrite(errorPin,HIGH);
  digitalWrite(leftPin,HIGH);
  digitalWrite(centerPin,HIGH);
  digitalWrite(rightPin,HIGH);
  analogWrite(motor_r, stopSpeed);
  analogWrite(motor_l, stopSpeed);
  delay(100);
  Brakes(Brakes1,brakeTight, UPDATE_TIME);
  Brakes(Brakes2,brakeTight, UPDATE_TIME);
  delay(100);
}
void Brakes(int servoPin,int pulse,int UPDATE_TIME){
  digitalWrite(servoPin,HIGH);
  delayMicroseconds(pulse);
  digitalWrite(servoPin,LOW);
  delay(UPDATE_TIME);
}
