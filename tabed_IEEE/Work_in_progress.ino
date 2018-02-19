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



///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////PID control for ramp/////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////




float elapsedTime, time, timePrev;
int i;


float PID, pwmBLeft, pwmFRight, error, previous_error, total_speed, desired_speed;
float pid_p=0;
float pid_i=0;
float pid_d=0;
/////////////////PID CONSTANTS/////////////////
double kp=3.55;//3.55
double ki=0.005;//0.003
double kd=2.05;//2.05
///////////////////////////////////////////////

double throttle=100; //initial value of throttle to the motors
//desired_speed = totaldistancefront*elapsedTime;

void PID_setup() {
  time = millis(); //Start counting time in milliseconds
}
void PID_loop() {

/////////////////////////////I M U/////////////////////////////////////
    timePrev = time;  // the previous time is stored before the actual time read
    time = millis();  // actual time read
    elapsedTime = (time - timePrev) / 1000; 
   
    
  /*The tiemStep is the time that elapsed since the previous loop. 
   * This is the value that we will use in the formulas as "elapsedTime" 
   * in seconds. We work in ms so we haveto divide the value by 1000 
   to obtain seconds*/

  
 
  
/*///////////////////////////P I D///////////////////////////////////*/
/*Remember that for the balance we will use just one axis. I've choose the x angle
to implement the PID with. That means that the x axis of the IMU has to be paralel to
the balance*/

/*First calculate the error between the desired angle and 
*the real measured angle*/
error = total_speed - desired_speed;
    
/*Next the proportional value of the PID is just a proportional constant
*multiplied by the error*/

pid_p = kp*error;

/*The integral part should only act if we are close to the
desired position but we want to fine tune the error. That's
why I've made a if operation for an error between -2 and 2 degree.
To integrate we just sum the previous integral value with the
error multiplied by  the integral constant. This will integrate (increase)
the value each loop till we reach the 0 point*/
if(-3 <error <3)
{
  pid_i = pid_i+(ki*error);  
}

/*The last part is the derivate. The derivate acts upon the speed of the error.
As we know the speed is the amount of error that produced in a certain amount of
time divided by that time. For taht we will use a variable called previous_error.
We substract that value from the actual error and divide all by the elapsed time. 
Finnaly we multiply the result by the derivate constant*/

pid_d = kd*((error - previous_error)/elapsedTime);

/*The final PID values is the sum of each of this 3 parts*/
PID = pid_p + pid_i + pid_d;

/*We know taht the min value of PWM signal is 1000us and the max is 2000. So that
tells us that the PID value can/s oscilate more than -1000 and 1000 because when we
have a value of 2000us the maximum value taht we could sybstract is 1000 and when
we have a value of 1000us for the PWM sihnal, the maximum value that we could add is 1000
to reach the maximum 2000us*/
if(PID < -100)
{
  PID=-100;
}
if(PID > 100)
{
  PID=100;
}

/*Finnaly we calculate the PWM width. We sum the desired throttle and the PID value*/
pwmFRight = throttle + PID;
pwmBLeft = throttle - PID;


/*Once again we map the PWM values to be sure that we won't pass the min
and max values. Yes, we've already maped the PID values. But for example, for 
throttle value of 1300, if we sum the max PID value we would have 2300us and
that will mess up the ESC.*/
//Right
if(pwmFRight < 100)
{
  pwmFRight= 100;
}
if(pwmFRight > 200)
{
  pwmFRight=200;
}
//Left
if(pwmBLeft < 100)
{
  pwmBLeft= 100;
}
if(pwmBLeft > 200)
{
  pwmBLeft=200;
}

/*Finnaly using the servo function we create the PWM pulses with the calculated
width for each pulse*/

//left_prop.writeMicroseconds(pwmBLeft);
//right_prop.writeMicroseconds(pwmFRight);
previous_error = error; //Remember to store the previous error.

}//end of loop void

