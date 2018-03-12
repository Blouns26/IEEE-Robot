


// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability

//Left Encoder
Encoder frontEnc(19, 24);

//Right Encoder
Encoder backEnc(18,22);




long oldPositionfront  = -999;
long oldPositionback = -999;

float EncoderFront_loop(){
  float newPositionfront = frontEnc.read();
  float Total_revsfront = (-newPositionfront/1920.0);
  float totaldistancefront = Total_revsfront * 12.385433;
  
  
  if (newPositionfront != oldPositionfront) {
     oldPositionfront = newPositionfront;
     Serial.print(-newPositionfront);
     Serial.print("\n\t");
     Serial.print(totaldistancefront);Serial.print("inches"); 
     Serial.print("\t");
     Serial.print("Total revelutions front wheel:  ");
     Serial.print(-newPositionfront/1920.0);
     Serial.print("\t");
  }
  return totaldistancefront;
}
float EncoderBack_loop(){
  float newPositionback = backEnc.read();
  float Total_revsback = (newPositionback/1920.0);
  float totaldistanceback = Total_revsback * 12.385433;
  
  
  if (newPositionback != oldPositionback){
    oldPositionback = newPositionback;
    Serial.print(newPositionback);
    Serial.print("\n");
    Serial.print("\n\t");
    Serial.print(totaldistanceback);Serial.print("inches"); 
    Serial.print("\t");
    Serial.print("Total revelutions back wheel:  ");
    Serial.print(newPositionback/1920.0);
    Serial.print("\t");
  }
  return totaldistanceback;
}


