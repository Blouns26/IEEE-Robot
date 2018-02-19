

/////////////////////////////////////////////////////
//////////////////////GLOBAL////////////////////////
///////////////////////////////////////////////////
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <VL53L0X.h>
#include <math.h>
#include "Kalman.h" // Source: https://github.com/TKJElectronics/KalmanFilter
#include <Encoder.h>
#include <IRLibRecvPCI.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(8,9,4,5,6,7);

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

IRrecvPCI myReceiver(18);//pin number for the receiver
// TODO: Make calibration routine




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
int rsp = 135;
int fsp = 175;
int brsp = 75;
int sp = 50;//75
int ssp = 40;
int sssp = 25;
int center = 0;
int _decode = 0;

///////////////////////////////////////////////////////////////////////////
///////////////////////TEST VARIABLES FOR INFRARED SENSOR//////////////////
///////////////////////////////////////////////////////////////////////////
int A = 0;
int B = 0;
int C = 0;


//////////////////////////////////////////
///////////SETUP LOOP/////////////////////
//////////////////////////////////////////

void setup() {


  
  Serial.begin(9600);           // set up Serial library at 9600 bps  
  AFMS.begin();
  myReceiver.enableIRIn(); // Start the receiver
  Serial.println(F("Ready to receive IR signals"));
  // set up the LCD's number of columns and rows:
  lcd.begin(16,2);
  // Print a message to the LCD.
  lcd.print("waiting");
  //MPU_setup();
  //PID_setup();
///////////////////////////////////////
//////Time Of flight Sensor Setup//////
///////////////////////////////////////
 
 //VL53L0X_setup();
 

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
     //Decode_loop();
     int Stage_number = Decode_loop();
     //Serial.println(Stage_number);
     int stage_adv = 0;
while (stage_adv == 0){
     
     if (Stage_number == 0)
     {
        A=0;
        B=0;
        C=0;
        lcd.setCursor(0,1);
        lcd.print("000");
        stage_adv = 1;
      }
     else if(Stage_number == 1)
     {
        A = 0;
        B = 0;
        C = 1; 
        lcd.setCursor(0,1);  
        lcd.print("001");
        stage_adv = 1;
     }
      else if(Stage_number == 2)
      {
        A = 0;
        B = 1;
        C = 0;
        lcd.setCursor(0,1);
        lcd.print("010");
        stage_adv = 1;
      }
      else if(Stage_number == 3)
      {
        A = 0;
        B = 1;
        C = 1;
        lcd.setCursor(0,1);
        lcd.print("011");
        stage_adv = 1;
      }
      else if(Stage_number == 4)
      {
        A = 1;
        B = 0;
        C = 0;
        lcd.setCursor(0,1);
        lcd.print("100");
        stage_adv = 1;     
      }
      else if (Stage_number == 5)
      {
       A = 1;
       B = 0;
       C = 1; 
       lcd.setCursor(0,1);
       lcd.print("101");
       stage_adv = 1;
      }
      else if (Stage_number == 6)
      {
        A = 1;
        B = 1;
        C = 0;
        lcd.setCursor(0,1);
        lcd.print("110");
        stage_adv = 1;
      }
      else if (Stage_number == 7)
      {
        A = 1;
        B = 1;
        C = 1;
        lcd.setCursor(0,1);
        lcd.print("111");
        stage_adv = 1;
      }
      
}
    //VL53L0X_Loop();
    //Moving_average_Rear();
    //Moving_average_Front();
    //Moving_average_Left();
    //Moving_average_Right();
   //MPU_loop();
   // PID_loop();
   //Ramp_movement();
   // Encoder_loop();
   //move_forward(40);
    
 //////////////////////////////////////////////////////////////////////////
 ////////////////////////Setup to read IR Sensor///////////////////////////
 /////////////////////////////////////////////////////////////// 
}
 /*
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
  /*
   int z = 0;
   int time_now = millis();
   int period = 1000;
   
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
    
    Ramp_movement();
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


