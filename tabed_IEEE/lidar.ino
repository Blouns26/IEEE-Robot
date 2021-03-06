//#define HIGH_ACCURACY
#define LONG_RANGE

/////////Sensor Reading Function////////////

int distance_Front(){
  long unfiltered_distance_Front = 0;;
  long distance_Front = 0;;
  //Serial.print(sensor.readRangeContinuousMillimeters());
  if (Sensor1.timeoutOccurred()) { Serial.print("TIMEOUT");}
  unfiltered_distance_Front = Sensor1.readRangeContinuousMillimeters();
  if (unfiltered_distance_Front < 2000 && unfiltered_distance_Front > 0 && unfiltered_distance_Front != -2783)
  {
    distance_Front = unfiltered_distance_Front;
  }
  //Serial.println();
 return distance_Front;
}
int distance_Rear(){
  long distance_Rear = 0;
  long unfiltered_distance_Rear = 0;
  
  if (Sensor2.timeoutOccurred()) { Serial.print("TIMEOUT");}
  unfiltered_distance_Rear = Sensor2.readRangeContinuousMillimeters();
  if(unfiltered_distance_Rear < 2000 && unfiltered_distance_Rear > 0 && unfiltered_distance_Rear != -2783)
  {
    distance_Rear = unfiltered_distance_Rear;
  }
  return distance_Rear;
}
int distance_Right(){
  long distance_Right = 0;
  long unfiltered_distance_Right = 0;
  
  if (Sensor3.timeoutOccurred()) { Serial.print("TIMEOUT");}
  unfiltered_distance_Right = Sensor3.readRangeContinuousMillimeters();
  
  if(unfiltered_distance_Right < 2000 && unfiltered_distance_Right > 0 && unfiltered_distance_Right != -2783)
  {
    distance_Right = unfiltered_distance_Right;  
  }
  return distance_Right;
}
int distance_Left(){
  long distance_Left = 0;
  long unfiltered_distance_Left = 0;
  if (Sensor4.timeoutOccurred()) { Serial.print("TIMEOUT");}
  unfiltered_distance_Left = Sensor4.readRangeContinuousMillimeters();
  
  if(unfiltered_distance_Left < 2000 && unfiltered_distance_Left > 0 && unfiltered_distance_Left != -2783)
  {
    distance_Left = unfiltered_distance_Left;
  }
  return distance_Left;
}

int distance_Rear2()
  {
    long distance_Rear2;
    if(Sensor5.timeoutOccurred())
    {
      Serial.print("TIMEOUT");
    }
    distance_Rear2 = Sensor5.readRangeContinuousMillimeters();
    return distance_Rear2;
  }
////////////////////////////////////////////////////////////////////////////
////////////////Lidar Sensor Initial Setup Function/////////////////////////
////////////////////////////////////////////////////////////////////////////


void VL53L0X_setup()
{
  /*WARNING*/
  //Shutdown pins of VL53L0X ACTIVE-LOW-ONLY NO TOLERANT TO 5V will fry them
  pinMode(XSHUT_pin1, OUTPUT);
  pinMode(XSHUT_pin2, OUTPUT);
  pinMode(XSHUT_pin3, OUTPUT);
  pinMode(XSHUT_pin4, OUTPUT);
  pinMode(XSHUT_pin5, OUTPUT);
  pinMode(XSHUT_pin6, OUTPUT);
  
  Serial.begin(9600);
  Wire.begin();
  //Change address of sensor and power up next one
  Sensor6.setAddress(Sensor6_newAddress);
  pinMode(XSHUT_pin5, INPUT);
  delay(10); //For power-up procedure t-boot max 1.2ms "Datasheet: 2.9 Power sequence"
  Sensor5.setAddress(Sensor5_newAddress);
  pinMode(XSHUT_pin4, INPUT);
  delay(10);
  Sensor4.setAddress(Sensor4_newAddress);
  pinMode(XSHUT_pin3, INPUT);
  delay(10);
  Sensor3.setAddress(Sensor3_newAddress);
  pinMode(XSHUT_pin2, INPUT);
  delay(10);
  Sensor2.setAddress(Sensor2_newAddress);
  pinMode(XSHUT_pin1, INPUT);
  delay(10);
  
  Sensor1.init();
  Sensor2.init();
  Sensor3.init();
  Sensor4.init();
  Sensor5.init();
  Sensor6.init();
  
  Sensor1.setTimeout(500);
  Sensor2.setTimeout(500);
  Sensor3.setTimeout(500);
  Sensor4.setTimeout(500);
  Sensor5.setTimeout(500);
  Sensor6.setTimeout(500);

  #if defined LONG_RANGE
      
      // increase laser pulse periods (defaults are 14 and 10 PCLKs)
      Sensor1.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
      Sensor1.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
      Sensor2.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
      Sensor2.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
      Sensor3.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
      Sensor3.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
      Sensor4.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
      Sensor4.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
  #endif
  
 // #if defined HIGH_ACCURACY
 //   Increase timing budget to 20 ms (defaukt is about 33 ms)
 //   Sensor1.setMeasurementTimingBudget(200000);
 // #endif
 
  // Start continuous back-to-back mode (take readings as
  // fast as possible).  To use continuous timed mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
  Sensor1.startContinuous();
  Sensor2.startContinuous();
  Sensor3.startContinuous();
  Sensor4.startContinuous();
  Sensor5.startContinuous();
  Sensor6.startContinuous();
}


void VL53L0X_Loop(){
  Serial.print(Sensor1.readRangeContinuousMillimeters());
  Serial.print(',');
  Serial.print(Sensor2.readRangeContinuousMillimeters());
  Serial.print(',');
  Serial.print(Sensor3.readRangeContinuousMillimeters());
  Serial.print(','); //Did not work for me result was 65535
  Serial.print(Sensor4.readRangeContinuousMillimeters());
  Serial.print(',');
  Serial.print(Sensor5.readRangeContinuousMillimeters());
  Serial.print(',');
  Serial.print(Sensor6.readRangeContinuousMillimeters());
  Serial.println();
}




