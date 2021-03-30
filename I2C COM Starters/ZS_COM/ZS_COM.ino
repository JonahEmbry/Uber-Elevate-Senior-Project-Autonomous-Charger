// Written by Jonah Embry
// Updated 3/26/2021
// This the communications code for the Z-Stage of the Uber Elevate Senior Capstone Design Project: Autonomous Charging Vehicle
// This code waits for confirmation of position from the Long Range System, then elevates to the appropriate height, then sends
// confirmation back to the Long Range System
// A status LED is included to display states and assist in trouble shooting

// Include the required Wire library for I2C<br>
#include <Wire.h>

int LED = 7;

char LRSDone = '0';
char ZSDone = '0';
 

void setup() {
  
  Serial.begin(9600);     //Serial Line for Trouble Shooting
  pinMode (LED, OUTPUT);  // Define the LED pin as Output

  //Communication Stuff
  Wire.begin(8);          // Start the I2C Bus as Slave on address 8
  Wire.onReceive(receiveEvent); // Attach a function to trigger when something is received.
  Wire.onRequest(requestEvent); // register event -> When LRS asks for update, need to give status of Z-Stage (0 = not ready, 1 = ready)
  
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
  
  
  if (LRSDone == '1') {        //Char received is 1, LRS is in position and it is time to elevate 
    digitalWrite(LED, HIGH);   //Blink LED Indicator
    delay(200);
    digitalWrite(LED, LOW);
    delay(200);
    digitalWrite(LED, HIGH);  //Keep LED Indicator On

    //Z-Stage Driving Code Goes Here:






    //Z-Stage is in position
    ZSDone = '1';
    
    
  }
}
