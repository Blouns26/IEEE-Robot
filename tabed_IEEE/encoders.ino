int encoder(){
  //Serial.println("this is working");
   n = digitalRead(encoder0PinA);
  if ((encoder0PinALast == LOW) && (n == HIGH)) {
    if (digitalRead(encoder0PinB) == LOW) {
      encoder0Pos--;
    }
    else {
      encoder0Pos++;
    }
    Serial.println (encoder0Pos);
    Serial.print ("/***");
     encoder0PinALast = n;
     return encoder0Pos;
 }
}

