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
int d = 0;
int stage[3] = {0};
int F = 0;
char str[3];

IRrecvPCI myReceiver(18);//pin number for the receiver

void setup(){

  Serial.begin(9600);// set up Serial library at 9600 bps  
  Serial3.begin(9600);
  myReceiver.enableIRIn(); // Start the receiver
  Serial.println(F("Ready to receive IR signals"));
  // set up the LCD's number of columns and rows:
  lcd.begin(16,2);
  // Print a message to the LCD.
  lcd.print("Waiting 4 Map");
  
  }

void loop() 
{
   int center = 0;
   while (center == 0)
   {
    
    int Stage_adv = 0;
    
   //Continue looping until you get a complete signal received   
  if (myReceiver.getResults()) { 
    
  }
    Serial.println(F("Do a cut-and-paste of the following lines into the "));
    Serial.println(F("designated location in rawSend.ino"));
    Serial.print(F("\n#define RAW_DATA_LEN "));
    Serial.println(recvGlobal.recvLength,DEC);
    Serial.print(F("uint16_t rawData[RAW_DATA_LEN]={\n\t"));
    for(bufIndex_t i=1;i<recvGlobal.recvLength;i++) {
      Serial.print(recvGlobal.recvBuffer[i],DEC);
      Serial.print(F(", "));
      //Serial.print("The size of RAW_DATA_LEN =  ");
      //Serial.println(sizeof(recvGlobal.recvBuffer));
      //Serial.println(i);
      if( (i % 8)==0) Serial.print(F("\n\t"));
          
    }
    Serial.println(F("1000};"));//Add arbitrary trailing space
    int test = recvGlobal.recvBuffer[1];
    Serial.print("This is the first bit and should be close to 9000:   ");
    Serial.println(test);
if (recvGlobal.recvLength == 20){
    if ( sizeof(recvGlobal.recvBuffer) >= 20)
    {
      int c = recvGlobal.recvBuffer[18];
      int b = recvGlobal.recvBuffer[16];
      int a = recvGlobal.recvBuffer[14];
      int d = recvGlobal.recvBuffer[12];
      //Serial.print(A);Serial.print('\t');Serial.print(B);Serial.print('\t');Serial.print(C);Serial.print('\n');
      
      if (d > 1000)
      {
        //Serial.println('1');
        stage[0] = 1;
      }
      else
      {
        //Serial.println('0');
        stage[0] = 0;
      }
      if (a > 1000)
      {
        //Serial.println('1');
        stage[1] = 1;
      }
      else
      {
      //Serial.println('0');
      stage[1] = 0;
      }
      if (b > 1000)
      {
        //Serial.println('1');
        stage[2] = 1;
      }
      else 
      {
        //Serial.println('0');
        stage[2] = 0;
      }
      if (c > 1000)
      {
        //Serial.println('1');
        stage[3] = 1;
      }
      else 
      {
        //Serial.println('0');
        stage[3] = 0;
      }
    }
    else if (sizeof(recvGlobal.recvBuffer) < 20)
    {
      lcd.setCursor(0,1);
      lcd.print("Error in code");
      Serial.print("Error with the number of bits");
    }
    
     F = (stage[3])+(stage[2]*(2))+(stage[1]*(4));
     //Serial.print("This is F: "); Serial.println(F);
     Serial.print(stage[1]);Serial.print('\t');Serial.print(stage[2]);Serial.print('\t'); Serial.println(stage[3]);
     //delay(10);
    
   
   Serial.print("This is the value of F: "); Serial.println(F);
   //delay(100);
   //itoa(F, str, 10);
   Serial.print("This is the value to send:  ");Serial.println(str);


   
   if(stage[0] == 1)
   {
   lcd.setCursor(0,1);
   lcd.print("positioning");
   }
   else
   {
    //lcd.print("           ");
    Serial3.write(F+48);
    Serial.println("Reading IR values"); 
    if (F == 0)
      {
        A=0;
        B=0;
        C=0;
        lcd.setCursor(0,1);
        lcd.print("0                               ");
        Stage_adv = 1;
        center = 0;
        break;
      }
      if(F == 1)
      {
        A = 0;
        B = 0;
        C = 1; 
        lcd.setCursor(0,1);  
        lcd.print("1                               ");
        Stage_adv = 1;
        center = 1;
        break;
     }
      else if(F == 2)
      {
        A = 0;
        B = 1;
        C = 0;
        lcd.setCursor(0,1);
        lcd.print("2                               ");
        //Stage_adv = 1;
        center = 1;
        break;
      }
      else if(F == 3)
      {
        A = 0;
        B = 1;
        C = 1;
        lcd.setCursor(0,1);
        lcd.print("3                               ");
        //Stage_adv = 1;
        center = 1;
        break;
      }
      else if(F == 4)
      {
        A = 1;
        B = 0;
        C = 0;
        lcd.setCursor(0,1);
        lcd.print("4                               ");
        //Stage_adv = 1;
        center = 1;
        break;     
      }
      else if (F == 5)
      {
       A = 1;
       B = 0;
       C = 1; 
       lcd.setCursor(0,1);
       lcd.print("5                               ");
       //Stage_adv = 1;
       center = 1;
       break;
      }
      else if (F == 6)
      {
        A = 1;
        B = 1;
        C = 0;
        lcd.setCursor(0,1);
        lcd.print("6                               ");
        //Stage_adv = 1;
        center = 1;
        break;
      }
      else if (F == 7)
      {
        A = 1;
        B = 1;
        C = 1;
        lcd.setCursor(0,1);
        lcd.print("7                               ");
        Stage_adv = 1;
        break;
      }
   }
}   
else
{
  Serial.println("Failed to send the 9ms pulse");
  lcd.setCursor(0,1);
  lcd.print("Failed    ");   
}

   //delay(1000);
     
    //Restart receiver
    myReceiver.enableIRIn(); 
  } 
 
}

