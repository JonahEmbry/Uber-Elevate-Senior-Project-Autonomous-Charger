/* CRS Controller
 *  by Jonah Embry
 *  12/21- Currently, WthetaI is outputting nan, which is messing 
 *  stuff up. Probably test the math in a different program, than
 *  import it back in. Likely one of the trig functions doesn't 
 *  work the way I want it to. Also, not sure if the tip offset
 *  is actually correct.
*/

#include <Servo.h>
#include <math.h>

Servo ArmServo;  // create servo object to control a servo
Servo BaseServo;
Servo TipServo;
// twelve servo objects can be created on most boards

const double TarX = 7;
const double TarY = 7;
const double InterX = 1;
const double Length = 8;
const double CharLeng = 2;
const double TotLength = Length + CharLeng;

double StartB;
double StartA;
double StartT;

double HomeB = 0;
double HomeA = 0;
double HomeT = 0;


double ThetaPortR;
double ThetaPortD;

double InY;
double InX;

const float pi = 3.14159267;

double ThetaIR;
double ThetaID;
double dxI;
double dxF;

double ThetaFR;
double ThetaFD;


void setup() {

  Serial.begin(9600);
  
  ArmServo.attach(8);  // attaches the servo on pin 9 to the servo object
  TipServo.attach(9);  // attaches the servo on pin 9 to the servo object
  BaseServo.attach(7);  // attaches the servo on pin 9 to the servo object

  StartB = BaseServo.read();
  StartA = ArmServo.read();
  StartT = TipServo.read();
}

void loop() {

  //Initial Positioning ********************************************************

      //Position Base Servo so arm is vertical
      if(StartB <= HomeB){
          for (int pos = StartB; pos <= HomeB; pos += 1) { // goes from 0 degrees to 180 degrees
            // in steps of 1 degree
            BaseServo.write(pos);              // tell servo to go to position in variable 'pos'
            delay(15);                       // waits 15ms for the servo to reach the position
            Serial.println(pos);
          } 
      }
      else {
          for (int pos = StartB; pos >= HomeB; pos -= 1) { // goes from 0 degrees to 180 degrees
            // in steps of 1 degree
            BaseServo.write(pos);              // tell servo to go to position in variable 'pos'
            delay(15);                       // waits 15ms for the servo to reach the position
            Serial.println(pos);
          } 
      }
  Serial.println("Base is Positioned");
  delay(2000);
  

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
  
      //Find Initial Position *****************************************************************
      if(TarX > InterX){
        ThetaPortR = atan(TarY/(TarX-InterX));  //Angle between x axis and path to x-intercept
        ThetaPortD = (180 * ThetaPortR)/pi;
      }
      else {
        ThetaPortR = atan(TarY/(InterX-TarX));  //Angle between x axis and path to x-intercept
        ThetaPortD = 180 - ((180 * ThetaPortR)/pi);
      }
      
  
  Serial.println("ThetaPortD");
  Serial.println(ThetaPortD);
  delay(2000);

   
  InY = TarY - (CharLeng * cos(ThetaPortR));  //Initial Y Position
  InX = TarX - (CharLeng * sin(ThetaPortR));  //Initial X Position
  Serial.println("Finding Initial Position");
  delay(2000);

  ThetaIR = asin(InY/Length);  //Initial angle for arm between x-axis and intial position
  ThetaID = (180 * ThetaIR)/pi; //Angle in degrees
  Serial.println("ThetaID");
  Serial.println(ThetaID);
  dxI = InX + (Length * cos(ThetaIR));               //Initial change in position
  Serial.println("dxI");
  Serial.println(dxI);
  delay(2000);

    // Move along X-axis **************************************************************************


    // Rotate Arm **************************************************************************
    Serial.println("Rotating Arm");
    delay(2000);

    if(TarX >= 0){
      for (int pos = HomeA; pos <= ThetaID; pos += 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        //ArmServo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
        Serial.println(pos);
      }      
    }
    else {
      for (int pos = HomeA; pos <= (180 - ThetaID); pos += 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        //ArmServo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
        Serial.println(pos);
      }
    }



  //Rotate Tip *****************************************************************************
    Serial.println("Rotating Tip");
    Serial.println(ThetaID);
    Serial.println(ThetaPortD);
    Serial.println(HomeT);
    delay(2000);
    

    if(TarX >= 0){
      if(ThetaID >= ThetaPortD){
        for (int pos = HomeT; pos <= (ThetaID - ThetaPortD); pos += 1) { // goes from 0 degrees to 180 degrees
          // in steps of 1 degree
          TipServo.write(pos);              // tell servo to go to position in variable 'pos'
          delay(15);                       // waits 15ms for the servo to reach the position
          Serial.println(pos);
        }
      }
      else {
        for (int pos = HomeT; pos >= (ThetaID - ThetaPortD); pos -= 1) { // goes from 0 degrees to 180 degrees
          // in steps of 1 degree
          TipServo.write(pos);              // tell servo to go to position in variable 'pos'
          delay(15);                       // waits 15ms for the servo to reach the position
          Serial.println(pos);
        }
      }
    } 
    else {
      if ((180 - ThetaID) >= ThetaPortD){
        for (int pos = HomeT; pos <= ((180 - ThetaID) - ThetaPortD); pos += 1) { // goes from 0 degrees to 180 degrees
          // in steps of 1 degree
          TipServo.write(pos);              // tell servo to go to position in variable 'pos'
          delay(15);                       // waits 15ms for the servo to reach the position
          Serial.println(pos);
        }
      }
      else {
        for (int pos = HomeT; pos >= ((180 - ThetaID) - ThetaPortD); pos -= 1) { // goes from 0 degrees to 180 degrees
          // in steps of 1 degree
          TipServo.write(pos);              // tell servo to go to position in variable 'pos'
          delay(15);                       // waits 15ms for the servo to reach the position
          Serial.println(pos);
        }
      }
    }



  //Interpolate to Final Position
  ThetaFR = (TarY/Length);    //Angle between x-axis and robotic arm in final position
  ThetaFD = (180 * ThetaFR)/pi;

    Serial.println("Interpolate");
    Serial.println("************************");
    delay(2000);

  //Change in Position
  if(TarX >= 0){
    dxF = TarX - (Length * cos(ThetaFR));
  } 
  else {
    dxF = TarX + (Length * cos(ThetaFR));
  }


  if(ThetaID <= ThetaFD){
    for (int pos = ThetaID; pos <= ThetaFD; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      
      // Move along X-axis set amount
      
      //ArmServo.write(pos);              // tell servo to go to position in variable 'pos'
      Serial.println(pos);
      delay(15);                       // waits 15ms for the servo to reach the position
      //TipSero.write(pos - ThetaPortD);
      Serial.println(pos - ThetaPortD);
      delay(15);                       // waits 15ms for the servo to reach the position
    }
  }
  else {
    for (int pos = ThetaID; pos <= ThetaFD; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      
      // Move along X-axis set amount
      
      //ArmServo.write(pos);              // tell servo to go to position in variable 'pos'
      Serial.println(pos);
      delay(15);                       // waits 15ms for the servo to reach the position
      //TipSero.write(pos - ThetaPortD);
      Serial.println(pos - ThetaPortD);
      delay(15);                       // waits 15ms for the servo to reach the position
    }
  }


delay(5000);
  
  
    
}
