


void moveLinear(int Position){
  if(Position == 180)
  {
    for (int pos = 0; pos <= 150; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    linear.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    }
  }
  else if(Position == 0)
  {
  for (int pos = 150; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    linear.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  }
  
}



