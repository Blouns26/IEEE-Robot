/*
void square()
{
    int x = 0;
   
   while (x == 0)  
   {   
      double distance1 = Moving_average_Rear_left();
      double distance2 = Moving_average_Rear_right();
   
      double distance3 = (distance1/distance2);
      double distance4 = (distance2/distance1);
      Serial.print("This is distance1/distance2:   ");
      Serial.println(distance3);
      Serial.print("This is distance2/distance1:   ");
      Serial.println(distance4);

      if ( distance3 >= .80 && distance3 <= 1.2 && distance4 <= 1.20 && distance4 >=.8)
      {
          Serial.print("Stopping");
          Stop(sp);
          x = 1;
      }

      
      else if (distance3 > 1.20 && distance4 < .80)
      {
        Serial.println("Moving to square left");
        square_left(sssp);
        delay(10);  
      }
      else if (distance3 < .80 && distance4 > 1.20 )
      {
        square_right(sssp);
        Serial.println("Moving to square right");
        delay(10);
      }
      
   }
}

*/

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////Moving Average Test for lidar data//////////////////////
///////////////////////////////////////////////////////////////////////////////////////

///////Back sensor//////// 
const int numReadings_rear = 5;
int readings_rear[numReadings_rear];    // the readings from the input
int readIndex_rear = 0;            // the index of the current reading
int total_rear =0;                 // the running total 
///////Left sensor///////
const int numReadings_left = 5;
int readings_left[numReadings_left];  // the readings from left input
int readIndex_left = 0;               // the index of the current reading from left sensor      
int total_left = 0;                   // the running total of the left sensor
///////Right sensor///////
const int numReadings_right = 5;
int readings_right[numReadings_right];
int readIndex_right = 0;
int total_right = 0;
//////Front sensor////////
const int numReadings_front = 5;
int readings_front[numReadings_front];
int readIndex_front = 0;
int total_front = 0;

int average_Rear = 0;         // the average
int average_Front = 0;
int average_Left = 0;
int average_Right = 0;

void Moving_average_Rear_setup()
{
  for (int thisReading_rear = 0; thisReading_rear < numReadings_rear; thisReading_rear++)
  {
    readings_rear[thisReading_rear] = 0;
  }
}

void Moving_average_Left_setup()
{
  for (int thisReading_left = 0; thisReading_left < numReadings_left; thisReading_left++)
  {
    readings_left[thisReading_left] = 0;
  }
}

void Moving_average_Right_setup()
{
  for (int thisReading_right = 0; thisReading_right < numReadings_right; thisReading_right++)
  {
    readings_right[thisReading_right] = 0;
  }
}

void Moving_average_Front_setup()
{
  for(int thisReading_front = 0; thisReading_front < numReadings_front; thisReading_front++)
  {
    readings_front[thisReading_front] = 0;
  }
}


int Moving_average_Rear()
{
  
  total_rear = total_rear - readings_rear[readIndex_rear];       //subtract the last reading;
  readings_rear[readIndex_rear] = distance_Rear();     //read data from the sensor
  total_rear = total_rear + readings_rear[readIndex_rear];       // add the reading to the total
  readIndex_rear = readIndex_rear + 1;                 // advance to next position in array

  if (readIndex_rear >= numReadings_rear)
  {
    readIndex_rear = 0; //wrap around to the beginning
  }

  average_Rear = total_rear / numReadings_rear;
  delay(1);
  return average_Rear;
}

int Moving_average_Front()
{
  
  total_front = total_front - readings_front[readIndex_front];       //subtract the last reading;
  readings_front[readIndex_front] = distance_Front();     //read data from the sensor
  total_front = total_front + readings_front[readIndex_front];       // add the reading to the total
  readIndex_front = readIndex_front + 1;                 // advance to next position in array

  if (readIndex_front >= numReadings_front)
  {
    readIndex_front = 0; //wrap around to the beginning
  }

  average_Front = total_front / numReadings_front;
  delay(1);
  return average_Front;
}
int Moving_average_Left()
{
  total_left = total_left - readings_left[readIndex_left];       //subtract the last reading;
  readings_left[readIndex_left] = distance_Left();     //read data from the sensor
  total_left = total_left + readings_left[readIndex_left];       // add the reading to the total
  readIndex_left = readIndex_left + 1;                 // advance to next position in array

  if (readIndex_left >= numReadings_left)
  {
    readIndex_left = 0; //wrap around to the beginning
  }

  average_Left = total_left / numReadings_left;
  delay(1);
  return average_Left;

}

int Moving_average_Right()
{
  total_right = total_right - readings_right[readIndex_right];       //subtract the last reading;
  readings_right[readIndex_right] = distance_Right();     //read data from the sensor
  total_right = total_right + readings_right[readIndex_right];       // add the reading to the total
  readIndex_right = readIndex_right + 1;                 // advance to next position in array

  if (readIndex_right >= numReadings_right)
  {
    readIndex_right = 0; //wrap around to the beginning
  }

  average_Right = total_right / numReadings_right;
  delay(1);
  return average_Right;

}



























