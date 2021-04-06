/* Written by Jonah Embry and Jaker Berkowitz
 *  Updated: 4/6/2021
 * 
 */

const int BUFFER_SIZE = 100;
char buf[BUFFER_SIZE];

char temp[8];
int tempcheck = 0;

float Tx; //Need
float Ty; //Need
float Tz; //Need
float Rx; //I want it
float Ry; //Need
float Rz; //I want it

 void setup() {
   // put your setup code here, to run once:
   Serial.begin(19200);
 }
 
 void loop() {
   // put your main code here, to run repeatedly:
   if (Serial.available()) {
//     // Read the most recent byte
//     byte byteRead = Serial.read();
//     // ECHO the value that was read
//     Serial.write(byteRead);

    // read the incoming bytes:
    int rlen = Serial.readBytesUntil('\n', buf, BUFFER_SIZE);

    // prints the received data
    Serial.print("I received: ");
    for(int i = 0; i < rlen; i++){
      Serial.println(buf[i]);
    }

    Serial.println("Starting my code *****************");
    delay(500);
    
    for(int i = 4; i <= 11; i++){
      temp[tempcheck] = buf[i];
      Serial.println(temp[tempcheck]);
      tempcheck++; 
    }
    
    Tx = atof(temp);

    Serial.print("Final Value: ");
    Serial.println(Tx + 10);
    delay(10000);
  
  }
}
