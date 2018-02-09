

#include "IRLibAll.h"
//#include <IRremote.h>

IRsend irsend;

IRrecvPCI myReceiver(2);
 
//Create a decoder object 
IRdecode myDecoder;   
 
void setup() {
  Serial.begin(9600);
  delay(2000); while (!Serial); //delay for Leonardo
  myReceiver.enableIRIn(); // Start the receiver
  Serial.println(F("Ready to receive IR signals"));
}


 
void loop() {
  //Continue looping until you get a complete signal received
  if (myReceiver.getResults()) {
    myDecoder.decode();           //Decode it
    myDecoder.dumpResults(true);  //Now print results. Use false for less detail
    myReceiver.enableIRIn();      //Restart receiver
    delay(50);
  }
}
