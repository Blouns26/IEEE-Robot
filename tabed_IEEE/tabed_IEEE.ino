/////////////////////////////////////////////////////
//////////////////////GLOBAL////////////////////////
///////////////////////////////////////////////////
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <VL53L0X.h>
#include <math.h>
#include "Kalman.h" // Source: https://github.com/TKJElectronics/KalmanFilter

#define RESTRICT_PITCH // Comment out to restrict roll to ±90deg instead - please read: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf

Kalman kalmanX; // Create the Kalman instances
Kalman kalmanY;

/* IMU Data */
double accX, accY, accZ;
double gyroX, gyroY, gyroZ;
int16_t tempRaw;

double gyroXangle, gyroYangle; // Angle calculate using the gyro only
double compAngleX, compAngleY; // Calculated angle using a complementary filter
double kalAngleX, kalAngleY; // Calculated angle using a Kalman filter

uint32_t timer;
uint8_t i2cData[14]; // Buffer for I2C data

// TODO: Make calibration routine


//////////Encoders///////////
int val;
int encoder0PinA = 26;
int encoder0PinB = 28;
int encoder0Pos = 0;
int encoder0PinALast = LOW;
int n = LOW;
int y = 0;
int period = 2000;
unsigned long time_now = 0;
int flag_move_rdy = 0;

////////////////////////////////////////////////////////////
///////////////////LIDAR INITIALIZATION/////////////////////
////////////////////////////////////////////////////////////

#define XSHUT_pin6 41
#define XSHUT_pin5 39
#define XSHUT_pin4 37
#define XSHUT_pin3 35
#define XSHUT_pin2 33
#define XSHUT_pin1 31

//ADDRESS_DEFAULT 0b0101001 or 41
#define Sensor1_newAddress 41 
#define Sensor2_newAddress 42
#define Sensor3_newAddress 43
#define Sensor4_newAddress 44
#define Sensor5_newAddress 45
#define Sensor6_newAddress 46

VL53L0X Sensor1;
VL53L0X Sensor2;
VL53L0X Sensor3;
VL53L0X Sensor4;
VL53L0X Sensor5;
VL53L0X Sensor6;


/////////////////////////////////////////////////////////////////////////
///////////////////////////MOTOR INITIALIZATION//////////////////////////
/////////////////////////////////////////////////////////////////////////

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Select which 'port' M1, M2, M3 or M4.
Adafruit_DCMotor *motor1 = AFMS.getMotor(1);
Adafruit_DCMotor *motor2 = AFMS.getMotor(2);
Adafruit_DCMotor *motor3 = AFMS.getMotor(3);
Adafruit_DCMotor *motor4 = AFMS.getMotor(4);
int rsp = 150;
int sp = 100;//75
int ssp = 75;
int sssp = 50;
int center = 0;

///////////////////////////////////////////////////////////////////////////
///////////////////////TEST VARIABLES FOR INFRARED SENSOR//////////////////
///////////////////////////////////////////////////////////////////////////
int A =rand() % 2;
int B = rand() % 2;
int C = rand() % 2;


//////////////////////////////////////////
///////////SETUP LOOP/////////////////////
//////////////////////////////////////////

void setup() {


  pinMode (encoder0PinA, INPUT);
  pinMode (encoder0PinB, INPUT);
  
  Serial.begin(9600);           // set up Serial library at 9600 bps  
  AFMS.begin();
  MPU_setup();

///////////////////////////////////////
//////Time Of flight Sensor Setup//////
///////////////////////////////////////
 
 VL53L0X_Setup();

////////////////////////////////////////
/////////////Motor Setup////////////////
////////////////////////////////////////

// Set the speed to start, from 0 (off) to 255 (max speed)
  motor1->setSpeed(0);
  motor1->run(FORWARD);
  motor1->run(RELEASE);
  motor2->setSpeed(0);
  motor2->run(BACKWARD);
  motor2->run(RELEASE);
  motor3->setSpeed(0);
  motor3->run(FORWARD);
  motor3->run(RELEASE);
  motor4->setSpeed(0);
  motor4->run(BACKWARD);
  motor4->run(RELEASE);

////////////////////////////////////////////////////////////////////////
//////////////Setup smoothing for distance sensors//////////////////////
////////////////////////////////////////////////////////////////////////
 
  Moving_average_Rear_setup();
  Moving_average_Left_setup();
  Moving_average_Right_setup();
  Moving_average_Front_setup();

}
/////////////////////////////////////////
////////////VOID LOOP START//////////////
/////////////////////////////////////////

void loop() 
{
  
  //VL53L0X_Loop();
  //encoder();
  //delay(100);
    Moving_average_Rear();
    Moving_average_Front();
    Moving_average_Left();
    Moving_average_Right();
    //MPU_loop();
    Ramp_movement();
    
}

  /*
  Serial.println("Raw Sensor value Back sensor");
  Serial.println(distance_Rear());
  Serial.println("Raw Sensor value Right sensor");
  Serial.println(distance_Right());
  Serial.println("Raw Sensor value Front senor");
  Serial.println(distance_Front());
  Serial.println("Raw Sensor value Left sensor");
  Serial.println(distance_Left());
  //delay(1000); 
  Serial.println("Moving Average left sensor");
  Serial.println(Moving_average_Left());
  Serial.println("Moving Average back sensor");
  Serial.println(Moving_average_Rear()); 
  Serial.println("Moving Average Right sensor");
  Serial.println(Moving_average_Right());
  Serial.println("Moving Average Front sensor");
  Serial.println(Moving_average_Front());
  //delay(1000);
 //Centering2();

*/
/*
 //////////////////////////////////////////////////////////////////////////
 ////////////////////////Setup to read IR Sensor///////////////////////////
 /////////////////////////////////////////////////////////////// 
 int center = 1; 
  while(center == 1 )
  {
  Centering();        // First Centering operation to make sure it is located on IR sensor
  center=2;
  }
  
  /////////////////////////////////////////////////////////////////////////
  /////////////////////////Stage One Code//////////////////////////////////
  /////////////////////////////////////////////////////////////////////////
  while(center==2)
  {
  Serial.print("moving to button");
  codemoveA();                          // After the IR sensor is read, the first value is added to variable A
  center=3;                             // This is the Movement for the first stage to get the key 
  }
  /////////////////////////////////////////////////////////////////////////
  ///////////////////Center robot to got down ramp/////////////////////////
  /////////////////////////////////////////////////////////////////////////
  while(center==3)
  {
    //Serial.print(" 2nd centering");       // This is the 2nd center operation to line up with the ramp
    Centering();
   // while (y == 0)
   // {
   //   square();
   //   y = 1;
   // }   
  center=4;
  }
  move_forward(sssp);                   // First move down the ramp
  delay(3000);
  Stop(sp); 
  Centering2();                         // Center operation to make sure it is now alligned with the bottom walls
  delay(1000);

  /////////////////////////////////////////////////////////////////////////////
  ///////////////////////////Move to stage 2///////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  while (center==4)
  {
  codemoveB();                          // 2nd IR value B is used to determine which location is next for stage 2
  center = 5;
  }
  
  /////////////////////////////////////////////////////////////////////////////
  ///////////////////////////Move to center with chest/////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  while (center == 5)
  {
    Centering2();                       // Center operation to line up with the treasure chest to pick up
    center = 6;
  }

  /////////////////////////////////////////////////////////////////////////////
  ////////////////////////////Move forward to chest////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  while (center == 6)
  {
    Serial.println("Moving to chest");
    chestmove(); 
    center = 7;
  }
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////Move to flag////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  while (center == 7)
  {  
    Serial.print("Moving to flag");
    flagmove();
    center = 8;
  }
  
  
  /////////////////////////////////////////////////////////////////////////////
  ////////////////////////////Move to ramp ////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  while (center == 8)
   {
  
   int z = 0;
   time_now = millis();
   
   while (z == 0)
    {     
       Serial.println("Moving to Ramp");
      if (Moving_average_Front() < 900)
      {  
        move_backward(ssp);
        Serial.print(Moving_average_Front()|| millis() < (time_now + period));
        if (distance_Front() >= 900)
        {
          Serial.print(Moving_average_Front());
          Stop(sp);
          Centering2();
          z = 1;
        }
      }
        
    }
    center = 9;
  }
  ///////////////////////////////////////////////////////////////////////////////
  ////////////////////////////Go up ramp/////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////
  while (center == 9)
  {
    Ramp_movement();
    center = 10;
  }
  ///////////////////////////////////////////////////////////////////////////////
  ////////////////////////////Finish the last stage//////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////
  
  while (center == 10)
  {
    codemoveC();
    Stop(sp);
    center = 0;
  }
  
  ////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////IR test////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////  
  if (A == 0 || B == 0 || C == 0)
  {
    A = A+1;
    B = B+1;
    C = C+1;
  }
  else{
    A = A-1;
    B = B-1;
    C = C-1; 
  }
  
}
///////////////////////////////////////////////////////////////
////////////////////Code for testing only//////////////////////
///////////////////////////////////////////////////////////////
/*
   while (center == 0)
  {
    chestmove();
    center = 1;
  }
  while (center == 1)
  {
  flagmove();
  center = 2;
  }
}
*/


