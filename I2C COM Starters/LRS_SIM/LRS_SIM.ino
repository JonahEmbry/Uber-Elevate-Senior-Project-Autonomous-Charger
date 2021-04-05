// Written by Jonah Embry
// Last Updated: 3/26/2021
// This is the overall code for the LRS Communication Protocols
// In this system, the LRS Mega fuctions as the Master: Z-stage (Device 8) and (CRS Device 9) are slaves
// This version of the code is meant purely to plug the charger in. We can worry about uplugging it later
// Include the required Wire library for I2C<br>
// MAKE SURE ALL BOARDS SHARE COMMON GROUND


#include <Wire.h>

int LED = 8;      //LRS Indicator LED
char CRSDone = '0';  //CRS flag variabe: if 0, CRS is not plugged in; if 1, CRS is plugged in
char ZSDone = '0';

void setup() {
// REMEMBER THAT THE ARDUINOS NEED TO SHARE A COMMON GROUND OR IT WILL NOT WORK (READ THE TOP NEXT TIME DUMBASS)  
  Serial.begin(9600);  // Serial Line for Trouble Shooting
  pinMode(LED, OUTPUT); // LRS Indicator LED
    
  // Communications Stuff
  Wire.begin();           // Start the I2C Bus as Master
  
}
void loop() {
      
      //LRS to CRS: Plug in the charger
      Serial.println("Transmitting");
      digitalWrite(LED, HIGH);
      delay(500);
      digitalWrite(LED, LOW);
      Wire.beginTransmission(8); // transmit to device #9 (Close Range System) or #8 (Z-Stage System)
      Wire.write('1');              // sends 1 
      Wire.endTransmission();    // stop transmitting


      //CRS to LRS: Charger Plugged in
    
      while (ZSDone != '1') {    // Waiting for CRS to confirm plug in
        Serial.println("Waiting for CRS Confirmation ");
        Wire.requestFrom(8, 1);                       // request 1 bytes from slave device #9 (CRS) or #8 Z-Stage
        ZSDone = Wire.read();          // receive a byte as character
        Serial.print(CRSDone);         // print the character
        //Blink LRS LED to display waiting
        digitalWrite(LED, HIGH);
        Serial.println();
        delay(1000);                  // Checks if CRSDone has been triggered every second
        digitalWrite(LED, LOW);
        delay(1000);
      }

      //LRS to CRS: Plug in the charger
      Serial.println("Transmitting");
      digitalWrite(LED, HIGH);
      delay(500);
      digitalWrite(LED, LOW);
      Wire.beginTransmission(9); // transmit to device #9 (Close Range System) or #8 (Z-Stage System)
      Wire.write('1');              // sends 1 
      Wire.endTransmission();    // stop transmitting


      //CRS to LRS: Charger Plugged in
    
      while (CRSDone != '1') {    // Waiting for CRS to confirm plug in
        Serial.println("Waiting for CRS Confirmation ");
        Wire.requestFrom(9, 1);                       // request 1 bytes from slave device #9 (CRS) or #8 Z-Stage
        CRSDone = Wire.read();          // receive a byte as character
        Serial.print(CRSDone);         // print the character
        //Blink LRS LED to display waiting
        digitalWrite(LED, HIGH);
        Serial.println();
        delay(1000);                  // Checks if CRSDone has been triggered every second
        digitalWrite(LED, LOW);
        delay(1000);
      }

      //At this point, the CRS should be plugged in
      Serial.println("DONE");
      digitalWrite(LED, HIGH);
      delay(10000);
    
}
    
    
