/* Written by Jonah Embry and Jaker Berkowitz
 *  Updated: 4/6/2021
 * 
 */

const int BUFFER_SIZE = 100;
char buf[BUFFER_SIZE];

char temp[8];
int flag = 0;

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
    for(int i = 0; i < rlen; i++){
      //Serial.println(buf[i]);
      if(buf[i] == ' ' && (buf[i-1] == ':' || buf[i-1] == 'x' || buf[i-1] == 'y' || buf[i-1] == 'z')){
        switch(flag){
          case(5):
                for(int ii = i + 1; ii <= i + 8; ii++){
                  temp[ii - (i+1)] = buf[ii];
                  Serial.print(temp[ii -(i+1)]);
                }
                Serial.println(" Case Five");
                Rz = atof(temp);
                flag = 6;
                break;
                                
          case(4):
                for(int ii = i + 1; ii <= i + 8; ii++){
                  temp[ii - (i+1)] = buf[ii];
                  Serial.print(temp[ii -(i+1)]);
                }
                Serial.println(" Case Four");
                Ry = atof(temp);
                flag = 5;
                break;
                                
          
          case(3):
                for(int ii = i + 1; ii <= i + 8; ii++){
                  temp[ii - (i+1)] = buf[ii];
                  Serial.print(temp[ii -(i+1)]);
                }
                Serial.println(" Case Three");
                Rx = atof(temp);
                flag = 4;
                break;
                            

          case(2):
                for(int ii = i + 1; ii <= i + 8; ii++){
                  temp[ii - (i+1)] = buf[ii];
                  Serial.print(temp[ii -(i+1)]);
                }
                Serial.println(" Case Two");
                Tz = atof(temp);
                flag = 3;
                break;
                
          
          case(1):
                for(int ii = i + 1; ii <= i + 8; ii++){
                  temp[ii - (i+1)] = buf[ii];
                  Serial.print(temp[ii -(i+1)]);
                }
                Serial.println(" Case One");
                Ty = atof(temp);
                flag = 2;
                break;
                                
          
          case(0):
                for(int ii = i + 1; ii <= i + 8; ii++){
                  temp[ii - (i+1)] = buf[ii];
                  Serial.print(temp[ii -(i+1)]);
                }
                Serial.println(" Case Zero");
                Tx = atof(temp);
                flag = 1;
                break;
                
        }
      }
      //Serial.println("Why am I here");
      
    }
    Serial.print("Final Values- Tx: ");
    Serial.print(Tx);
    Serial.print(" Ty: ");
    Serial.print(Ty);
    Serial.print(" Tz: ");
    Serial.print(Tz);
    Serial.print(" Rx: ");
    Serial.print(Rx);
    Serial.print(" Ry: ");
    Serial.print(Ry);
    Serial.print(" Rz: ");
    Serial.println(Rz);
    flag = 0;

  
    Serial.println("*******Done***********");
    delay(10000);
  
  }
}
