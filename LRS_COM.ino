// Written by Jonah Embry
// Last Updated: 3/26/2021
// This is the overall code for the LRS Communication Protocols
// In this system, the LRS Mega fuctions as the Master: Z-stage (Device 8) and (CRS Device 9) are slaves
// This version of the code is meant purely to plug the charger in. We can worry about uplugging it later
// Include the required Wire library for I2C<br>


#include <Wire.h>
#include <RedBot.h>

//Declare IR sensors
RedBotSensor left_sen = RedBotSensor(A2); 
RedBotSensor center_sen = RedBotSensor(A1);
RedBotSensor right_sen = RedBotSensor(A0); 

//motor right
#define motor_r 7
//motor left
#define motor_l 8
int lineStandard = 1000;

//Motor Speeds
int fullSpeed = 110;
int turnSpeed = 80;
int stopSpeed = 0;

//LED pins
#define errorPin 38 //RED
#define successPin 30 //Green
#define leftPin 34 //BLUE
#define centerPin 32 //BLUE
#define rightPin 30 //BLUE

//pins for Ultrasonic sensors
#define trigPin1 13
#define echoPin1 12
#define trigPin2 4
#define echoPin2 5


int LED = 40;      //LRS Indicator LED
char ZDone = '0';    //Z-stage flag variable: if 0, z-stage is not elevated; if 1, z-stage is elevated
char CRSDone = '0';  //CRS flag variabe: if 0, CRS is not plugged in; if 1, CRS is plugged in

char user_input;  //Variable for user input for the start

void setup() {
  //outputs for motors
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
  
  Serial.begin(9600);  // Serial Line for Trouble Shooting
  pinMode(LED, OUTPUT); // LRS Indicator LED
    
  // Communications Stuff
  Wire.begin();           // Start the I2C Bus as Master
}
void loop() {
    user_input = Serial.read(); //Reads Serial1, which should be used in serial monitor. If user enters 1, the system starts

    if(user_input == '1'){      //Start program

      //Start LRS Movement Function
         
while(true){        

  //IR values
  int left = left_sen.read();
  int center = center_sen.read(); 
  int right = right_sen.read();

  //Ultrasonic Left
  long duration1, distance1;  
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = (duration1/2) / 29.1; //centimeters


  //ultrasonic right
  long duration2, distance2;  
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = (duration2/2) / 29.1; //centimeters

  //Print distance and IR values
  Serial.print(distance1);
  Serial.print(" cm");
  Serial.print("\t");
  Serial.print(distance2);
  Serial.print(" cm");
  Serial.print("\t");
  Serial.print(left);
  Serial.print("\t");  
  Serial.print(center);
  Serial.print("\t");
  Serial.print(right);
  Serial.println();

  if(distance1<60 || distance2<60){
    digitalWrite(successPin,LOW);
    digitalWrite(errorPin,HIGH);
    digitalWrite(leftPin,LOW);
    digitalWrite(centerPin,LOW);
    digitalWrite(rightPin,LOW);  
    analogWrite(motor_r, stopSpeed);
    analogWrite(motor_l, stopSpeed);  
  }
    
  bool qtil= left>lineStandard;
  bool qtim= center>lineStandard;
  bool qtir= right>lineStandard;

 if(qtim && !qtir && !qtil){          //qti on the line in the middle
  digitalWrite(successPin,LOW);
  digitalWrite(errorPin,LOW);
  digitalWrite(leftPin,LOW);
  digitalWrite(centerPin,HIGH);
  digitalWrite(rightPin,LOW);
  analogWrite(motor_r, fullSpeed);
  analogWrite(motor_l, fullSpeed);
  }
 else if(qtir && !qtim && !qtil){     //only line on the right
  digitalWrite(successPin,LOW);
  digitalWrite(errorPin,LOW);
  digitalWrite(leftPin,LOW);
  digitalWrite(centerPin,LOW);
  digitalWrite(rightPin,HIGH);
  analogWrite(motor_r, turnSpeed);
  analogWrite(motor_l, fullSpeed);
  break;
  }
 else if(qtil && !qtim && !qtir){     //only on the left
  digitalWrite(successPin,LOW);
  digitalWrite(errorPin,LOW);
  digitalWrite(leftPin,HIGH);
  digitalWrite(centerPin,LOW);
  digitalWrite(rightPin,LOW);
  analogWrite(motor_r, fullSpeed);
  analogWrite(motor_l, turnSpeed);
  }
  else if(qtil && qtim && qtir){    //reaches the end
  digitalWrite(successPin,HIGH);
  digitalWrite(errorPin,LOW);
  digitalWrite(leftPin,LOW);
  digitalWrite(centerPin,LOW);
  digitalWrite(rightPin,LOW); 
  analogWrite(motor_r, stopSpeed);
  analogWrite(motor_l, stopSpeed); 
  //break; 
  } 
  else{                                //detects nothing and stops                           
  digitalWrite(successPin,LOW);
  digitalWrite(errorPin,HIGH);
  digitalWrite(leftPin,LOW);
  digitalWrite(centerPin,LOW);
  digitalWrite(rightPin,LOW);  
  analogWrite(motor_r, stopSpeed);
  analogWrite(motor_l, stopSpeed);  
  }
}
Serial.println("done");
      //LRS to Z-Stage: Start elevation
      Wire.beginTransmission(8); // transmit to device #8 (Z-Stage)
      Wire.write('1');              // sends 1 
      Wire.endTransmission();    // stop transmitting


      //Z-stage to LRS: Finished elevation
          //Used code from here: https://www.arduino.cc/en/Tutorial/LibraryExamples/MasterReader
  
      while (ZDone == '0') {    // waiting for Z-Stage to confirm elevation
        Serial1.println("Waiting for Z-Confirmation ");
        Wire.requestFrom(8, 1);                       // request 1 bytes from slave device #8 (Z-stage)
        ZDone = Wire.read();          // receive a byte as character
        Serial1.print(ZDone);         // print the character
        //Blink LRS LED to display waiting
        Serial1.println();
        delay(1000);                  // Checks if ZDone has been triggered every second
      }

        //At this point, Z-Stage should be in position


      //LRS to CRS: Plug in the charger
      Wire.beginTransmission(9); // transmit to device #9 (Close Range System)
      Wire.write('1');              // sends 1 
      Wire.endTransmission();    // stop transmitting


      //CRS to LRS: Charger Plugged in
    
      while (CRSDone == '0') {    // Waiting for CRS to confirm plug in
        Serial1.println("Waiting for CRS Confirmation ");
        Wire.requestFrom(9, 1);                       // request 1 bytes from slave device #9 (CRS)
        CRSDone = Wire.read();          // receive a byte as character
        Serial1.print(CRSDone);         // print the character
        //Blink LRS LED to display waiting
        Serial1.println();
        delay(1000);                  // Checks if CRSDone has been triggered every second
      }

      //At this point, the CRS should be plugged in
      
    }
}
    
    
