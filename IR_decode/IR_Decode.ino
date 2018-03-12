/* rawR&cv.ino Example sketch for IRLib2
 *  Illustrate how to capture raw timing values for an unknow protocol.
 *  You will capture a signal using this sketch. It will output data the 
 *  serial monitor that you can cut and paste into the "rawSend.ino"
 *  sketch.
 */

// Recommend only use IRLibRecvPCI or IRLibRecvLoop for best results
#include <IRLibRecvPCI.h> 
#include <LiquidCrystal.h>
LiquidCrystal lcd(8,9,4,5,6,7);

int A = 0;
int B = 0;
int C = 0;

int a = 0;
int b = 0;
int c = 0;
int stage[2] = {0};
int F = 0;

IRrecvPCI myReceiver(18);//pin number for the receiver

void setup(){

  Serial.begin(9600);// set up Serial library at 9600 bps  
  Serial3.begin(9600);
  myReceiver.enableIRIn(); // Start the receiver
  Serial.println(F("Ready to receive IR signals"));
  // set up the LCD's number of columns and rows:
  lcd.begin(16,2);
  // Print a message to the LCD.
  lcd.print("waiting");
  
  }

void loop() 
{
   //Continue looping until you get a complete signal received   
  if (myReceiver.getResults()) { 
    //Serial.println(F("Do a cut-and-paste of the following lines into the "));
    //Serial.println(F("designated location in rawSend.ino"));
    //Serial.print(F("\n#define RAW_DATA_LEN "));
    //Serial.println(recvGlobal.recvLength,DEC);
    //Serial.print(F("uint16_t rawData[RAW_DATA_LEN]={\n\t"));
    for(bufIndex_t i=1;i<recvGlobal.recvLength;i++) {
      //Serial.print(recvGlobal.recvBuffer[i],DEC);
      //Serial.print(F(", "));
      if( (i % 8)==0) Serial.print(F("\n\t"));
          
    }
    //Serial.println(F("1000};"));//Add arbitrary trailing space
       
    if ( sizeof(recvGlobal.recvBuffer) >= 20)
    {
      int c = recvGlobal.recvBuffer[18];
      int b = recvGlobal.recvBuffer[16];
      int a = recvGlobal.recvBuffer[14];
      //Serial.print(A);Serial.print('\t');Serial.print(B);Serial.print('\t');Serial.print(C);Serial.print('\n');
      
      if (a > 1000)
      {
        //Serial.println('1');
        stage[0] = 1;
      }
      else
      {
        //Serial.println('0');
        stage[0] = 0;
      }
      if (b > 1000)
      {
        //Serial.println('1');
        stage[1] = 1;
      }
      else
      {
      //Serial.println('0');
      stage[1] = 0;
      }
      if (c > 1000)
      {
        //Serial.println('1');
        stage[2] = 1;
      }
      else 
      {
        //Serial.println('0');
        stage[2] = 0;
      }
    }
     for (int i = 0; i <= 2; i++)
    {
       
       //Serial.print(stage[i]);
     }
       //Serial.print("\n");
    
     F = (stage[2])+(stage[1]*(2))+(stage[0]*(4));
     //Serial.print("This is F: "); Serial.println(F);
     Serial.print(stage[0]);Serial.print('\t');Serial.print(stage[1]);Serial.print('\t'); Serial.println(stage[2]);
     //delay(10);
    
    myReceiver.enableIRIn();      //Restart receiver
  } 
  Serial.print("This is the value of F: "); Serial.println(F);
  delay(500);
  Serial3.print(F);
}

