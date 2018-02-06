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



/*MPU-6050 gives you 16 bits data so you have to create some 16int constants
 * to store the data for accelerations and gyro*/

int16_t Acc_rawX, Acc_rawY, Acc_rawZ,Gyr_rawX, Gyr_rawY, Gyr_rawZ;
float elapsedTime, time, timePrev;
int i;

//float Acceleration_angle[2];
//float Gyro_angle[2];
float Total_Acceleration[2];

float PID, pwmFLeft, pwmFRight, error, previous_error;
float pid_p=0;
float pid_i=0;
float pid_d=0;
/////////////////PID CONSTANTS/////////////////
double kp=3.55;//3.55
double ki=0.005;//0.003
double kd=2.05;//2.05
///////////////////////////////////////////////

double throttle=100; //initial value of throttle to the motors
float desired_Acceleration = -1; //This is the angle in which we whant the
                         //balance to stay steady


void PID_setup() {
  Wire.begin(); //begin the wire comunication
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  
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

  /*Reed the values that the accelerometre gives.
   * We know that the slave adress for this IMU is 0x68 in
   * hexadecimal. For that in the RequestFrom and the 
   * begin functions we have to put this value.*/
   
     Wire.beginTransmission(0x68);
     Wire.write(0x3B); //Ask for the 0x3B register- correspond to AcX
     Wire.endTransmission(false);
     Wire.requestFrom(0x68,6,true); 
   
   /*We have asked for the 0x3B register. The IMU will send a brust of register.
    * The amount of register to read is specify in the requestFrom function.
    * In this case we request 6 registers. Each value of acceleration is made out of
    * two 8bits registers, low values and high values. For that we request the 6 of them  
    * and just make then sum of each pair. For that we shift to the left the high values 
    * register (<<) and make an or (|) operation to add the low values.*/
    
     Acc_rawX=Wire.read()<<8|Wire.read(); //each value needs two registres
     Acc_rawY=Wire.read()<<8|Wire.read();
     Acc_rawZ=Wire.read()<<8|Wire.read();

 
    /*///This is the part where you need to calculate the angles using Euler equations///*/
    
    /* - Now, to obtain the values of acceleration in "g" units we first have to divide the raw   
     * values that we have just read by 16384.0 because that is the value that the MPU6050 
     * datasheet gives us.*/
    /* - Next we have to calculate the radian to degree value by dividing 180º by the PI number
    * which is 3.141592654 and store this value in the rad_to_deg variable. In order to not have
    * to calculate this value in each loop we have done that just once before the setup void.
    */

    /* Now we can apply the Euler formula. The atan will calculate the arctangent. The
     *  pow(a,b) will elevate the a value to the b power. And finnaly sqrt function
     *  will calculate the rooth square.*/
     /*---X---*/
     //Acceleration_angle[0] = atan((Acc_rawY/16384.0)/sqrt(pow((Acc_rawX/16384.0),2) + pow((Acc_rawZ/16384.0),2)))*rad_to_deg;
     /*---Y---*/
     //Acceleration_angle[1] = atan(-1*(Acc_rawX/16384.0)/sqrt(pow((Acc_rawY/16384.0),2) + pow((Acc_rawZ/16384.0),2)))*rad_to_deg;
 
   /*Now we read the Gyro data in the same way as the Acc data. The adress for the
    * gyro data starts at 0x43. We can see this adresses if we look at the register map
    * of the MPU6050. In this case we request just 4 values. W don¡t want the gyro for 
    * the Z axis (YAW).*/
    
   Wire.beginTransmission(0x68);
   Wire.write(0x43); //Gyro data first adress
   Wire.endTransmission(false);
   Wire.requestFrom(0x68,4,true); //Just 4 registers
   
   Gyr_rawX=Wire.read()<<8|Wire.read(); //Once again we shif and sum
   Gyr_rawY=Wire.read()<<8|Wire.read();
 
   /*Now in order to obtain the gyro data in degrees/seconda we have to divide first
   the raw value by 131 because that's the value that the datasheet gives us*/

   /*---X---*/
//   Gyro_angle[0] = Gyr_rawX/131.0; 
   /*---Y---*/
//   Gyro_angle[1] = Gyr_rawY/131.0;

   /*Now in order to obtain degrees we have to multiply the degree/seconds
   *value by the elapsedTime.*/
   /*Finnaly we can apply the final filter where we add the acceleration
   *part that afects the angles and ofcourse multiply by 0.98 */


  ///Change Angle to Acceleration
  //  Total_AccelerationX = accX;
  //  Total_AccelerationY = accY;
   
   /*---X axis angle---*/
   //Total_angle[0] = 0.98 *(Total_angle[0] + Gyro_angle[0]*elapsedTime) + 0.02*Acceleration_angle[0];
   /*---Y axis angle---*/
  //Total_angle[1] = 0.98 *(Total_angle[1] + Gyro_angle[1]*elapsedTime) + 0.02*Acceleration_angle[1];
   
   /*Now we have our angles in degree and values from -10º0 to 100º aprox*/
    //Serial.println(Total_angle[1]);

   
  
/*///////////////////////////P I D///////////////////////////////////*/
/*Remember that for the balance we will use just one axis. I've choose the x angle
to implement the PID with. That means that the x axis of the IMU has to be paralel to
the balance*/

/*First calculate the error between the desired angle and 
*the real measured angle*/
error = Total_Acceleration[1] - desired_Acceleration;
    
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
pwmFLeft = throttle + PID;
pwmFRight = throttle - PID;


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
if(pwmFLeft < 100)
{
  pwmFLeft= 100;
}
if(pwmFLeft > 200)
{
  pwmFLeft=200;
}

/*Finnaly using the servo function we create the PWM pulses with the calculated
width for each pulse*/

//left_prop.writeMicroseconds(pwmLeft);
//right_prop.writeMicroseconds(pwmRight);
previous_error = error; //Remember to store the previous error.

}//end of loop void
