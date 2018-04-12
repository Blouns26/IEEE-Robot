

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
#include <Servo.h>
#include <NewPing.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>  // not used in this demo but required!

#define RESTRICT_PITCH // Comment out to restrict roll to Ãƒâ€šÃ‚Â±90deg instead - please read: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf
#define SONAR_NUM      4 // Number of sensors.
#define MAX_DISTANCE 299 // Maximum distance (in cm) to ping.
#define PING_INTERVAL 33 // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).

/*Compass*/
// i2c
Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();

#define LSM9DS1_SCK A5
#define LSM9DS1_MISO 12
#define LSM9DS1_MOSI A4
#define LSM9DS1_XGCS 6
#define LSM9DS1_MCS 5
// You can also use software SPI
//Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1(LSM9DS1_SCK, LSM9DS1_MISO, LSM9DS1_MOSI, LSM9DS1_XGCS, LSM9DS1_MCS);
// Or hardware SPI! In this case, only CS pins are passed in
//Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1(LSM9DS1_XGCS, LSM9DS1_MCS);



/*Sonic Sensor*/
unsigned long pingTimer[SONAR_NUM]; // Holds the times when the next ping should happen for each sensor.
unsigned int cm[SONAR_NUM];         // Where the ping distances are stored.
uint8_t currentSensor = 0;          // Keeps track of which sensor is active.
//int distance_right = cm[0];
//int distance_left = cm[1];
//int distance_front = cm[2];
//int distance_back  = cm[3];

/*Kalman Filter*/
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

/*Encoder Global Variables*/
    int new_Enc_back = 0;
    int new_Enc_front = 0;
////////////////////////////////////////////////////////////
///////////////////SONAR INITIALIZATION/////////////////////
////////////////////////////////////////////////////////////
NewPing sonar[SONAR_NUM] = {   // Sensor object array.
  NewPing(28, 30, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(32, 34, MAX_DISTANCE),
  NewPing(36, 38, MAX_DISTANCE),
  NewPing(40, 42, MAX_DISTANCE)
};   


////////////////////////////////////////////////////////////
///////////////////LIDAR INITIALIZATION/////////////////////
////////////////////////////////////////////////////////////

#define XSHUT_pin6 41
#define XSHUT_pin5 31
#define XSHUT_pin4 39
#define XSHUT_pin3 35
#define XSHUT_pin2 37
#define XSHUT_pin1 33

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

Servo linear;
/////////////////////////////////////////////////////////////////////////
///////////////////////////MOTOR INITIALIZATION//////////////////////////
/////////////////////////////////////////////////////////////////////////

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Select which 'port' M1, M2, M3 or M4.
Adafruit_DCMotor *motor1 = AFMS.getMotor(1);
Adafruit_DCMotor *motor2 = AFMS.getMotor(2);
Adafruit_DCMotor *motor3 = AFMS.getMotor(3);
Adafruit_DCMotor *motor4 = AFMS.getMotor(4);



/////////////////////////////////////////////////////////////////////////
////////////////////////Variable Decleration/////////////////////////////
/////////////////////////////////////////////////////////////////////////

/*Speed Variables*/
int rsp = 120;
int fsp = 120;
int brsp = 85;
int sp = 80;//75
int ssp = 50;
int sssp = 30;

/*Encoder variable*/


int _decode = 0;

///////////////////////////////////////////////////////////////////////////
///////////////////////TEST VARIABLES FOR INFRARED SENSOR//////////////////
///////////////////////////////////////////////////////////////////////////
int A = 0;
int B = 0;
int C = 0;

char str[3];

///////////////////////////////////////////////////////////////////////////
///////////////////////Variables for average AngleY////////////////////////
///////////////////////////////////////////////////////////////////////////
const int numReadings = 10;
double AngleYreadings[numReadings];
int readIndex = 0;
int total = 0;
double kalAngleY_avg = 0;


//////////////////////////////////////////
///////////SETUP LOOP/////////////////////
//////////////////////////////////////////

void setup() {



  Serial.begin(9600);    // set up Serial at 9600 bps  
  Serial3.begin(9600);   // set up Serial3 at 9600 bps
  linear.attach(9);
  AFMS.begin();
  MPU_setup();
  setup_Ping();
  KalAngleYavg_setup();
  
  //PID_setup();

///////////////////////////////////////
//////Time Of flight Sensor Setup//////
///////////////////////////////////////

  VL53L0X_setup();

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
  
    MPU_loop();
    
    //PID_loop();
    //move_forward(sp);    
   //Serial.print("This is the kalAngleY_avg:   ");
   //Serial.println(kalAngleYavg());
   //Serial.print("This is the kalAngleY:    ");
   //Serial.println(kalAngleY);
   //Serial.print("This is the pitch

    
 //////////////////////////////////////////////////////////////////////////
 ////////////////////////Setup to read IR Sensor///////////////////////////
 /////////////////////////////////////////////////////////////// 

int center = 0;
while (center ==0){
    int Stage_adv = 0;
    Serial.println("Reading data");
//    if(Serial3.available()){
    Serial3.readBytes(str,1); 
    delay(100);
    Serial.print("This is the first read: ");
    Serial.println(str);
      if (str[0] == '7')
      {
        A = 1;
        B = 1;
        C = 1;
        //Serial.print(a), Serial.print(b), Serial.print(c);
       center = 1;
      }
      
    if (str[0] == '6')
   {
    A = 1;
    B = 1;
    C = 0;
    //Serial.print(a), Serial.print(b), Serial.print(c);
    center = 1;
    }if (str[0] == '5')
   {
    A = 1;
    B = 0;
    C = 1;
    //Serial.print(a), Serial.print(b), Serial.print(c);
    center = 1;
    }
    if (str[0] == '4')
    {
      A = 1;
      B = 0;
      C = 0;
      //Serial.print(a), Serial.print(b), Serial.print(c);
      center = 1;
    }
    if (str[0] == '3')
    {
      A = 0;
      B = 1;
      C = 1;
      //Serial.print(a), Serial.print(b), Serial.print(c);
      center = 1;
    } 
    if (str[0] == '2')
    {
      A = 0;
      B = 1;
      C = 0;
      //Serial.print(a), Serial.print(b), Serial.print(c);
      center = 1;
    }
    if (str[0] == '1')
    {
      A = 0;
      B = 0;
      C = 1;
      //Serial.print(a), Serial.print(b), Serial.print(c);
      center = 1;
    }
    if (str[0] == '0')
    {
      A = 0;
      B = 0;
      C = 0;
      //Serial.print(a), Serial.print(b), Serial.print(c);
      
      center = 1;
    }
   
  
}
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
  moveLinear(180);
  moveLinear(50);
  center=3;                             // This is the Movement for the first stage to get the key 
  }
  /////////////////////////////////////////////////////////////////////////
  ///////////////////Center robot to got down ramp/////////////////////////
  /////////////////////////////////////////////////////////////////////////
  while(center==3)
  {
    Serial.println("2nd centering");       // This is the 2nd center operation to line up with the ramp
    Centering();
    int y = 1;
    while (y == 1)
    {
      MPU_loop();
      if (kalAngleY > -1 && kalAngleY < 2)
      {
          Serial.println("Moving down ramp");
          move_forward(sssp);    // First move down the ramp
          MPU_loop();          
      }
      else if (kalAngleY > 12 && kalAngleY < 30)
      {
        Serial.print("Transition to ramp");
        y = 2;
      }
     
    }
    while (y == 2)
    {
      MPU_loop();
      if (kalAngleY > 12 && kalAngleY < 30)
          {
            Serial.println("Now on ramp");
            move_forward(sssp);
            MPU_loop();
          }
      else if(kalAngleY > -1 && kalAngleY < 4)
            {
              Serial.println("Reached the bottom of the ramp"); 
              MPU_loop();
              Stop(sp); 
              y = 3;
            }         
     }
     while (y == 3)
     {
        Serial.println("2nd center operation");
        Centering2();  // Center operation to make sure it is now alligned with the bottom walls
        center=4;
        delay(1000);
        y = 0;
     }
  }

  /////////////////////////////////////////////////////////////////////////////
  ///////////////////////////Move to stage 2///////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  while (center==4)
  {
  new_Enc_back = 0;
  Serial.print("New Encoder value");
  Serial.print(new_Enc_back);
  codemoveB();    // 2nd IR value B is used to determine which location is next for stage 2
  delay(1000);
  //Centering2();
  center = 5;
  }
  
  /////////////////////////////////////////////////////////////////////////////
  ///////////////////////////Move to center with chest/////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  while (center == 5)
  {
    //Centering2();                       // Center operation to line up with the treasure chest to pick up
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
   int time_now = millis();
   int period = 1000;
   
   while (z == 0)
    {     
       Serial.println("Moving to Ramp");
       Serial.println("This the Front distance sensor:   ");
       Serial.print(front());
      if (front() < 1000)   
      {  
        Serial.println("Front distance sensor");
        Serial.println(front());
        Serial.println("Back distance sensor");
        Serial.print(back());
        move_backward(ssp);
        Serial.println("Moving backward");
        //Serial.print(Moving_average_Front()|| millis() < (time_now + period));
        if (front() >= 1000)
        {
          Serial.print(front());
          Serial.println("distance front is greater than 1000");
          Stop(sp);
          Centering2();
          z = 1;
        }
      }
        
    }
    Serial.println("Starting code to go up ramp");
    center = 9;
  }
  ///////////////////////////////////////////////////////////////////////////////
  ////////////////////////////Go up ramp/////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////
  while (center == 9)
  {
    Serial.println("Moving up ramp");
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
 
}

///////////////////////////////////////////////////////////////
////////////////////Code for testing only//////////////////////
///////////////////////////////////////////////////////////////






