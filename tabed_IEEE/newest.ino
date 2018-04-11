

void setup_Ping() {
  Serial.begin(9600);
  pingTimer[0] = millis() + 100;           // First ping starts at 75ms, gives time for the Arduino to chill before starting.
  for (uint8_t i = 1; i < SONAR_NUM; i++) // Set the starting time for each sensor.
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
}

void loop_Ping() {
  for (uint8_t i = 0; i < SONAR_NUM; i++) { // Loop through all the sensors.
    if (millis() >= pingTimer[i]) {         // Is it this sensor's time to ping?
      pingTimer[i] += PING_INTERVAL * SONAR_NUM;  // Set next time this sensor will be pinged.
      sonar[currentSensor].timer_stop();          // Make sure previous timer is canceled before starting a new ping (insurance).
      currentSensor = i;                          // Sensor being accessed.
      //cm[currentSensor] = 0;                      // Make distance zero in case there's no ping echo for this sensor.
      sonar[currentSensor].ping_timer(echoCheck); // Do the ping (processing continues, interrupt will call echoCheck to look for echo).
    }
  }
  // Other code that *DOESN'T* analyze ping results can go here.
}

void echoCheck() { // If ping received, set the sensor distance to array.
  if (sonar[currentSensor].check_timer()) {
    cm[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
    //pingResult(currentSensor);
  }
}
/*
void pingResult(uint8_t sensor) { // Sensor got a ping, do something with the result.
  // The following code would be replaced with your code that does something with the ping result.
  Serial.print(sensor);
  Serial.print(" ");
  Serial.print(cm[sensor]);
  Serial.print("cm");
  Serial.println();
  if(cm[1] < 50)
  {
   Serial.print("too close");
  }
  else
  {
    Serial.print("no error");
  }
}
*/
int right(){
   
   int distance_right = cm[0]*10;
   loop_Ping();
return distance_right;
}

int front(){
  
  int distance_front = cm[1]*10;
  loop_Ping();
  return distance_front;
}

int left(){
  int distance_left = cm[2]*10;
  loop_Ping();
  return distance_left;
}
int back(){
  int distance_back = cm[3]*10;
  loop_Ping();
  return distance_back;
}


