//This is opposite from Julia's
//Switching to a white versus black line
//Follows the line and turns into it rather than turn away

#include <RedBot.h>


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

//motor right
#define motor_r 44
//motor left
#define motor_l 45



//Redbot sensors
RedBotSensor left_sen = RedBotSensor(A0);  
RedBotSensor center_sen = RedBotSensor(A1);
RedBotSensor right_sen = RedBotSensor(A2);  
// constants that are used in the code. lineStandard is the level to detect
// if the sensor is on the line or not. If the sensor value is greater than this
// the sensor is above a DARK line.



// Various Constants
int lineStandard = 930;
const int turnSlowSpeed = 60;
const int turnFastSpeed = 100;
const int goSpeed = 70;
const int stopSpeed = 0;
const int StopDist = 60;
const int brakeTight = 1500;
const int brakeLoose = 1100;
int flag = 0;



void setup()
{
  //outputs for line following sensors
  pinMode(motor_r, OUTPUT);
  pinMode(motor_l, OUTPUT);

  //variables for ultrasonics
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  //variables for LEDs
  pinMode(errorPin,OUTPUT);
  pinMode(successPin,OUTPUT);
  pinMode(leftPin,OUTPUT);
  pinMode(centerPin,OUTPUT);
  pinMode(rightPin,OUTPUT);
 
  Serial.begin(9600);
   

  delay(1000);
}

void loop(){
  
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
  Serial.println();
 

//data_sheet
bool qtil = Left<lineStandard;
bool qtim = Center<lineStandard;
bool qtir = Right<lineStandard;

if (distance1<StopDist || distance2<StopDist){
  Stop();
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
else if(qtil && qtim && qtir){ //All white
  Success();
}
else{
  Stop();
}
  delay(10);  // add a delay to decrease sensitivity.
}

void right() {
  digitalWrite(successPin,HIGH);
  digitalWrite(errorPin,LOW);
  digitalWrite(leftPin,LOW);
  digitalWrite(centerPin,LOW);
  digitalWrite(rightPin,HIGH);
  //delay(100);
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

 
}
void left() {
  digitalWrite(successPin,HIGH);
  digitalWrite(errorPin,LOW);
  digitalWrite(leftPin,HIGH);
  digitalWrite(centerPin,LOW);
  digitalWrite(rightPin,LOW);
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
}
