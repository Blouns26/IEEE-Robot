
int Centering()
{
  Serial.println("Starting Centering operation");
  int x=0;
  while( x==0)
  {
   if (right() < 485)
   {
      move_left(ssp);
      Serial.println("moving left");
      Serial.print("This is the Right sensor:  ");
      Serial.println(right());
   }
   else if (left() < 505)
   {
      move_right(ssp);
      Serial.println("moving right");
      Serial.print("This is the Left sensor:  ");
      Serial.println(left());
   }
   else if (right() >= 450 || left() >= 450)
  {
      Serial.println("stop");
      Stop(ssp);
      delay(500);
      x=1;
  }
 }
 while (x==1)
 {
     if( back() > 135)
     {
          move_backward(sssp);
     }
     else if (back() < 128) 
     {
          move_forward(sssp);
     }
     else if (back() >=128 || back() <=135)
     {
         Stop(ssp);
         x=2;
     }
 }
}

int Centering2()
{
  Serial.println("Starting lower lv Centering operation");
  Serial.print("Right sensor:  ");
  Serial.print(right());
  Serial.print("Left sensor:   ");
  Serial.print(left());
  int x=0;
  while( x==0)
  {

    int y = 0;
      
    while (y == 0)
    {
      if (right() < 452)
      {
        move_left(ssp);
        Serial.println("moving left");
        Serial.print("Right distnace sensor: ");
        Serial.println(right());
      } 
      if (left() < 452)
      {
      move_right(ssp);
      Serial.println("moving right");
      Serial.print("Left distance sensor: ");
      Serial.println(left());
      }
      
      y=1;
     }
     if (right() >= 452 && left() >=452)
        {
          Serial.println("stop");
          //VL53L0X_Loop();
          Stop(ssp);
          delay(2000);
          x=1;
        }
  }
}
/*
void ramp()
{
  
  move_backward(rsp);
  delay(1800);
  if (back()() > 200)
    {
      move_backward(ssp);
      if (back()() <= 150)
        {
          Stop(sp);
          Centering();        
        }
    }
  
  center = 0;
}
*/

void Ramp_movement()
{
   
    Serial.println("Starting Ramp_Movement function");
    int z = 0;
    
    while (z == 0)              //When the robot is at the bottom of ramp
    {
      MPU_loop();
      if (kalAngleY > -1 && kalAngleY < 6)
        {
          Serial.print("Starting to move up ramp: ");
          MPU_loop();
          move_backward(sp);  
        }
      
       else if (kalAngleY > 8 && kalAngleY < 15)
        {
          Serial.print("This is the kalAngleY: ");
          Serial.println(kalAngleY);
          for (int i = 0; i < 1; i ++)
          { 
            Serial.println("Setting up to move up ramp");
            move_forward(ssp);
            i = i++;
            delay(200);
            
            Stop(sp);
            z = 1;
          }
         }
                  
      }
         while( z == 1)               //Moving to get on ramp
         {   
            int previous_reading = 0;
            MPU_loop();
            if (kalAngleY > -1 && kalAngleY < 6)
            {
              Serial.println("Moving up the ramp");
              move_backward_ramp();
              MPU_loop();
             }
             else if(kalAngleY > 10)
             {
              Serial.print("Robot is on the ramp");
              move_backward_ramp();
              MPU_loop();
              z = 2;
             }    
          }
          while (z == 2)
          {
           MPU_loop();
           if (kalAngleY > 15)
           {
            move_backward_ramp();    
            Serial.print("About to be on top of ramp");
           }
           else if (kalAngleY < 15)
           {
            if (back() > 200)
            {
              MPU_loop();
              move_backward(ssp);
            }
            else if (back() < 100)
            {
              Serial.print("The robot made it to the top safely");
              Stop(sp);
              z = 3;
            }
           }
    
     while (z == 3)
     {
      Centering();
      Serial.print("Leaving Ramp loop");
      z = 4;
     }
    }
}


/////////////////////////////////////
/////////////IR code reading/////////
////////////////////////////////////
void codemoveA(){

  Serial.println("Starting CodemoveA function");
  VL53L0X_Loop(); 
  int x =0;
  //int y=1;
  while(x==0){
  
  if (A==0){
  move_left(ssp);
  if(left() < 150){
  move_left(sssp);
    if (back() > 130)
    {
      move_backward(sssp);
    }
    else if (back() < 120)
    {
      move_forward(sssp);
    }
  if(left() <100){
  move_left(18);
  if(left() < 45){
    Stop(sp);
    delay(500);
    x=1;
     }
    }
   }
  }
  else if (A==1){
   move_right(ssp);
  if(right() <170){
  move_right(sssp);
    if (back() > 130){
      move_backward(sssp);
    }
    else if (back() < 120){
      move_forward(sssp);
    }
  if(right() <100){
  move_right(18);
  if(right() < 45){
    Stop(sp);
    delay(500);
    x=1;
     }
    }
   }
  }
 }
}


void codemoveB()
{
  Serial.println("Starting codemoveB function");
  //VL53L0X_Loop(); 
  int x =0;
  while(x==0)
  {
    int y=0;
    if (B==0)
    {
      move_topressure_left();
      x = 1;  
    }
   
   else if (B==1)
   {
    move_topressure_right();
    x = 1;
   }
  }    
}
  
 
void codemoveC(){
  Serial.println("Starting codemoveC function");
  //VL53L0X_Loop(); 
  int x =0;
  //int y=1;
  while(x==0){
  
  if (C==0){
  move_left(ssp);
  if(left() <150){
  move_left(sssp);
    if (back() > 190)
    {
      move_backward(sssp);
    }
    else if (back() < 170)
    {
      move_forward(sssp);
    }
  if(left() <100){
  move_left(18);
  if(left() < 85){
    Stop(sp);
    delay(500);
    x=1;
     }
    }
   }
  }
  else if (C==1){
   move_right(ssp);
  if(right() <170){
  move_right(sssp);
    if (back() > 190){
      move_backward(sssp);
    }
    else if (back() < 170){
      move_forward(sssp);
    }
  if(right() <100){
  move_right(18);
  if(right() < 85){
    Stop(sp);
    delay(500);
    x=1;
     }
    }
   }
  }
 }
}

void chestmove()
{
  Serial.println("Starting Chestmove function");
  //VL53L0X_Loop(); 
  int chest_exit = 0;
  while (chest_exit == 0)
  {
    
    if (front() > 700)
      {
        Serial.println("Moving to chest");
        move_forward(sssp);
      }
    else if (front() < 700)
     {
      Serial.print("Stopping at chest");
      Stop(sp);
      delay(1000);
      chest_exit = 1;
     } 
     
  }    
}

void flagmove()
{
  Serial.println("Starting flagmove function");
  
  int flag_exit = 0;
  while (flag_exit == 0)
  {
    
    if (front()> 300)
    {
      Serial.println("Moving to flag");
      move_forward(ssp);
      if (front() < 300 && front() > 180) 
      {
        move_forward(sssp);    
      }
    } 
    else if (front() <= 100)
      {
        Serial.print("Stopping at flag");
        Stop(sp);
        delay(1000);
        Centering2();
        flag_exit = 1;
      }
   
   }
  
}
/*void distance_test()
{
  Serial.println("Starting distance test function");
  
  int flag_exit = 0;
  while (flag_exit == 0)
  {
    if (left()()> 300)
    {
      Serial.println("Moving left");
      move_left(ssp);
      if (left()() < 300 && left()() > 180) 
      {
        move_left(sssp);    
      }
    } 
    else if (left()() <= 100)
      {
        Serial.print("Stopping at wall");
        Stop(sp);
        delay(10000);
        //Centering2();
        flag_exit = 1;
      }
   
   }
  
}*/


void move_topressure_left()
{
    
    EncoderBack_loop();
    float Enc_back =  EncoderBack_loop();
    float Enc_front = EncoderFront_loop(); 
    float prev_Enc_front = 0;
    float prev_Enc_back = 0;
    prev_Enc_back = Enc_back;
    prev_Enc_front = Enc_front;
    float back_error = 1;
    int go = 1;
    
    //Serial.print("This is the back encoder distance:  ");
    //Serial.println(Enc_back);
    //Serial.print("This is the front encoder distnace:  " );
    //Serial.println(Enc_front);
    while (go ==1)
    { 
      float Enc_back =  EncoderBack_loop(); 
    if (new_Enc_back < 14.39*back_error)
     {
      
      EncoderBack_loop();
      new_Enc_back = Enc_back - prev_Enc_back;
      Serial.print("This is the New encoder reading");
      Serial.println(new_Enc_back);
      move_forward(sssp);
      
     }
     else
     {
      Serial.print("Limit reached");
      new_Enc_back = Enc_back - prev_Enc_back;
      Serial.print("This is the new Encoder distance:  ");
      Serial.println(new_Enc_back);
      Stop(sp);
      go = 0;
     }
    }

    while (go == 0)
    { 
     if (left() > 200)
        {
          move_left(sp);
        }
        else if (left() <= 127) 
        {
         Stop(sp);
         Serial.print("Stop for 1 sec");
         delay(1000);
         go = 2;
        }
     
     }
    
    
}

void move_topressure_right()
{
    EncoderBack_loop(); 
    float Enc_back =  EncoderBack_loop();
    float Enc_front = EncoderFront_loop();
    float prev_Enc_front = 0;
    float prev_Enc_back = 0;
    prev_Enc_back = Enc_back;
    prev_Enc_front = Enc_front;
    int new_Enc_back = 0;
    int new_Enc_front = 0;
    float back_error = 1;
    int go = 1;
    
    //Serial.print("This is the back encoder distance:  ");
    //Serial.println(Enc_back);
    //Serial.print("This is the front encoder distnace:  " );
    //Serial.println(Enc_front);
    while (go ==1)
    { 
      float Enc_back =  EncoderBack_loop(); 
      if (new_Enc_back < 14.39*back_error)
     {
      EncoderBack_loop();
      new_Enc_back = Enc_back - prev_Enc_back;
      Serial.print("This is the New encoder reading");
      Serial.println(new_Enc_back);
      move_forward(sssp);
      }
     else
     {
      Serial.print("Limit reached");
      new_Enc_back = Enc_back - prev_Enc_back;
      Serial.print("This is the new Encoder distance:  ");
      Serial.println(new_Enc_back);
      Stop(sp);
      go = 0;
     }
    }

    while (go == 0)
    { 
     if (right() > 200)
        {
          move_right(sp);
        }
        else if (right() <= 127) 
        {
         Stop(sp);
         Serial.print("Stop for 1 sec");
         delay(1000);
         go = 2;
        }
     
     }
    
    
}
/*
float EncZeroFront()
{
    EncoderBack_loop();
    float Enc_front = EncoderFront_loop();
    float prev_Enc_front = 0;
    prev_Enc_front = Enc_front;
    int new_Enc_front = 0;
    new_Enc_front = Enc_front - prev_Enc_front;
    Serial.print("This is the new Encoder distance:  ");
    Serial.println(new_Enc_back);

return new_Enc_front
}
*/
/////////////////////////////////////////////////////////////////////////////
//////////////////////////Motor direction setup//////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// motors 1 and 4 are inverse directions                                                
void move_forward(int sp)                                                                   
{
  motor1->run(FORWARD);
  motor1->setSpeed(sp);
  motor2->run(BACKWARD);
  motor2->setSpeed(sp);                              
  motor3->run(FORWARD);
  motor3->setSpeed(sp);
  motor4->run(BACKWARD);
  motor4->setSpeed(sp+4);
}
// motors 1 and 4 are inverse driections
void move_backward(int sp)
{
  motor1->run(BACKWARD);
  motor1->setSpeed(sp);
  motor2->run(FORWARD);
  motor2->setSpeed(sp);
  motor3->run(BACKWARD);
  motor3->setSpeed(sp);
  motor4->run(FORWARD);
  motor4->setSpeed(sp+4);
}
// motors 1 and 4 are inveresed directions
void move_left(int sp)
{
  motor1->run(BACKWARD);
  motor1->setSpeed(sp);
  motor2->run(BACKWARD);
  motor2->setSpeed(sp);
  motor3->run(FORWARD);
  motor3->setSpeed(sp);
  motor4->run(FORWARD);
  motor4->setSpeed(sp+4);
}
//motors 1 and 4 are inveresed
void move_right(int sp)
{
  motor1->run(FORWARD);
  motor1->setSpeed(sp);
  motor2->run(FORWARD);
  motor2->setSpeed(sp);
  motor3->run(BACKWARD);
  motor3->setSpeed(sp);
  motor4->run(BACKWARD);
  motor4->setSpeed(sp+4);
}
//motors 1 and 4 are inversed
void move_diag_right(int sp)
{
  motor1->run(FORWARD);
  motor1->setSpeed(sp);
  motor2->run(RELEASE);
  motor2->setSpeed(sp);
  motor3->run(RELEASE);
  motor3->setSpeed(sp);
  motor4->run(BACKWARD);
  motor4->setSpeed(sp+4);
}
//motors 1 and 4 are inversed
void move_diag_left(int sp)
{
  motor1->run(RELEASE);
  motor1->setSpeed(sp);
  motor2->run(BACKWARD);
  motor2->setSpeed(sp);
  motor3->run(FORWARD);
  motor3->setSpeed(sp);
  motor4->run(RELEASE);
  motor4->setSpeed(sp+4);  
}

void Stop(int sp)
{
  motor1->run(FORWARD);
  motor1->setSpeed(0);
  motor2->run(BACKWARD);
  motor2->setSpeed(0);
  motor3->run(FORWARD);
  motor3->setSpeed(0);
  motor4->run(BACKWARD);
  motor4->setSpeed(0);  
}
//motors 1 and 4 are inversed
void square_left(int sp)
{
  motor1->run(BACKWARD);
  motor1->setSpeed(sp);
  motor2->run(BACKWARD);
  motor2->setSpeed(sp);
  motor3->run(BACKWARD);
  motor3->setSpeed(sp);
  motor4->run(BACKWARD);
  motor4->setSpeed(sp+4); 
  
}
//motors 1 and 4 are inversed
void square_right(int sp)
{
  motor1->run(BACKWARD);
  motor1->setSpeed(sp);
  motor2->run(BACKWARD);
  motor2->setSpeed(sp);
  motor3->run(BACKWARD);
  motor3->setSpeed(sp);
  motor4->run(BACKWARD);
  motor4->setSpeed(sp+4); 
  
}


void move_backward_ramp()
{
  motor1->run(BACKWARD);
  motor1->setSpeed(rsp);
  motor2->run(FORWARD);
  motor2->setSpeed(rsp);
  motor3->run(BACKWARD);
  motor3->setSpeed(rsp);
  motor4->run(FORWARD);
  motor4->setSpeed(rsp);
}






