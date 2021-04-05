// Written by Jonah Embry
// Last Updated: 3/26/2021
// This is the overall code for the LRS Communication Protocols
// In this system, the LRS Mega fuctions as the Master: Z-stage (Device 8) and (CRS Device 9) are slaves
// This version of the code is meant purely to plug the charger in. We can worry about uplugging it later
// Include the required Wire library for I2C<br>
// MAKE SURE ALL BOARDS SHARE COMMON GROUND


#include <Wire.h>

int LED = 8;      //LRS Indicator LED
char ZDone = '0';    //Z-stage flag variable: if 0, z-stage is not elevated; if 1, z-stage is elevated
char CRSDone = '0';  //CRS flag variabe: if 0, CRS is not plugged in; if 1, CRS is plugged in

char user_input;  //Variable for user input for the start

void setup() {
  
  Serial1.begin(9600);  // Serial Line for Trouble Shooting
  pinMode(LED, OUTPUT); // LRS Indicator LED
    
  // Communications Stuff
  Wire.begin();           // Start the I2C Bus as Master
  
}
void loop() {
    user_input = Serial1.read(); //Reads Serial1, which should be used in serial monitor. If user enters 1, the system starts

    if(user_input == '1'){      //Start program

      //Start LRS Movement Function
        
        //LRS Driving Code Goes Here:

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
    
    
