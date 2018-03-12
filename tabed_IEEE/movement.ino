
int Centering()
{
  Serial.println("Starting Centering operation");
  int x=0;
  while( x==0)
  {
   if (distance_Right() < 525)
   {
      move_left(ssp);
      Serial.println("moving left");
   }
   else if (distance_Left() < 525)
   {
      move_right(ssp);
      Serial.println("moving right");
   }
   else if (distance_Right() >= 525 || distance_Left() >= 525)
  {
      Serial.println("stop");
      Stop(ssp);
      delay(500);
      x=1;
  }
 }
 while (x==1)
 {
     if( distance_Rear() > 135)
     {
          move_backward(sssp);
     }
     else if (distance_Rear() < 128) 
     {
          move_forward(sssp);
     }
     else if (distance_Rear() >=128 || distance_Rear() <=135)
     {
         Stop(ssp);
         x=2;
     }
 }
}

int Centering2()
{
  Serial.println("Starting lower lv Centering operation");
  int x=0;
  while( x==0)
  {

    int y = 0;
      
    while (y == 0)
    {
      if (distance_Right() < 440)
      {
        move_left(ssp);
        Serial.println("moving left");
        
      } 
      if (distance_Left() < 440)
      {
      move_right(ssp);
      Serial.println("moving right");
      }
      
      y=1;
     }
     if (distance_Right() >= 445 && distance_Right() <=455)
        {
          Serial.println("stop");
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
  if (distance_Rear() > 200)
    {
      move_backward(ssp);
      if (distance_Rear() <= 150)
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
    int y = 0;
    
    while (y ==0)
    {
      MPU_loop();
      if (kalAngleX < 8)
        {
          Serial.println("starting to move up ramp");
          Serial.println(kalAngleX);
          MPU_loop();
          move_backward(sp);  
        }
      
       else if (kalAngleX >= 8)
        {
          for (int i = 0; i < 1; i ++)
          { 
            Serial.print("Setting up to move up ramp");
            move_forward(ssp);
            i = i++;
            delay(200);
            Stop(sp);
            y = 1;
          }
          
        }            
         while( y == 1)
         
         {   
            int previous_reading = 0;
            MPU_loop();
            if (kalAngleX < 7)
            {
            Serial.println("Moving up the ramp");
            move_backward_ramp();
            MPU_loop();
            previous_reading = kalAngleX;
            }
            
            if (previous_reading > 10 || distance_Rear() <= 190)
          {
            Serial.println("Reading back distance");
            Serial.println(distance_Rear());
            move_backward(sssp);
            if (distance_Rear() < 100)
            {
            Stop(sp);
            y = 2;
            }    
          }   
         }
      }
     while (y == 2)
     {
      //Centering();
      y == 2;
     }
    
}


/////////////////////////////////////
/////////////IR code reading/////////
////////////////////////////////////
void codemoveA(){

  Serial.println("Starting CodemoveA function");
  int x =0;
  //int y=1;
  while(x==0){
  
  if (A==0){
  move_left(ssp);
  if(distance_Left() <150){
  move_left(sssp);
    if (Moving_average_Rear() > 150)
    {
      move_backward(sssp);
    }
    else if (Moving_average_Rear() < 135)
    {
      move_forward(sssp);
    }
  if(distance_Left() <100){
  move_left(18);
  if(distance_Left() < 85){
    Stop(sp);
    delay(500);
    x=1;
     }
    }
   }
  }
  else if (A==1){
   move_right(ssp);
  if(distance_Right() <170){
  move_right(sssp);
    if (Moving_average_Rear() > 190){
      move_backward(sssp);
    }
    else if (Moving_average_Rear() < 170){
      move_forward(sssp);
    }
  if(distance_Right() <100){
  move_right(18);
  if(distance_Right() < 85){
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
  
  int x =0;
  while(x==0)
  {
    int y=0;
    if (B==0)
    {
      if (distance_Left() > 260)
      {
      move_diag_left(sp); 
      }
      else if (distance_Left() <= 260 )
      {
      Stop(sp);
      delay(500);
      }
      while (y==0)
      {
        if (distance_Front() > 900)
        {
          move_forward(sp);
          Stop(sp);
        }
        else if (distance_Front() < 870)
        {
          move_backward(sp);
          Stop(sp);
          
        }
       }
       
    }
   
   else if (B==1)
   {
    if (distance_Right() > 260)
    {
      move_diag_right(sp); 
    }
    else if (distance_Right() <= 260 )
    {
      Stop(sp);
      delay(500);
      x=1;
    }
   }
  }    
}
  
 
void codemoveC(){
  Serial.println("Starting codemoveC function");
  int x =0;
  //int y=1;
  while(x==0){
  
  if (C==0){
  move_left(ssp);
  if(distance_Left() <150){
  move_left(sssp);
    if (Moving_average_Rear() > 190)
    {
      move_backward(sssp);
    }
    else if (Moving_average_Rear() < 170)
    {
      move_forward(sssp);
    }
  if(distance_Left() <100){
  move_left(18);
  if(distance_Left() < 85){
    Stop(sp);
    delay(500);
    x=1;
     }
    }
   }
  }
  else if (C==1){
   move_right(ssp);
  if(distance_Right() <170){
  move_right(sssp);
    if (Moving_average_Rear() > 190){
      move_backward(sssp);
    }
    else if (Moving_average_Rear() < 170){
      move_forward(sssp);
    }
  if(distance_Right() <100){
  move_right(18);
  if(distance_Right() < 85){
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
  int chest_exit = 0;
  while (chest_exit == 0)
  {
    
    if (distance_Front() > 700)
      {
        Serial.println("Moving to chest");
        move_forward(sssp);
      }
    else if (distance_Front() < 700)
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
    
    if (distance_Front()> 300)
    {
      Serial.println("Moving to flag");
      move_forward(ssp);
      if (distance_Front() < 300 && distance_Front() > 180) 
      {
        move_forward(sssp);    
      }
    } 
    else if (distance_Front() <= 100)
      {
        Serial.print("Stopping at flag");
        Stop(sp);
        delay(1000);
        Centering2();
        flag_exit = 1;
      }
   
   }
  
}
void distance_test()
{
  Serial.println("Starting distance test function");
  
  int flag_exit = 0;
  while (flag_exit == 0)
  {
    if (distance_Left()> 300)
    {
      Serial.println("Moving left");
      move_left(ssp);
      if (distance_Left() < 300 && distance_Left() > 180) 
      {
        move_left(sssp);    
      }
    } 
    else if (distance_Left() <= 100)
      {
        Serial.print("Stopping at wall");
        Stop(sp);
        delay(10000);
        //Centering2();
        flag_exit = 1;
      }
   
   }
  
}
/////////////////////////////////////////////////////////////////////////////
//////////////////////////Motor direction setup//////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// motors 1 and 4 are inverse directions                                                
void move_forward(int sp)                                                                   
{
  motor1->run(BACKWARD);
  motor1->setSpeed(sp+3);
  motor2->run(FORWARD);
  motor2->setSpeed(sp);                              
  motor3->run(BACKWARD);
  motor3->setSpeed(sp);
  motor4->run(FORWARD);
  motor4->setSpeed(sp);
}
// motors 1 and 4 are inverse driections
void move_backward(int sp)
{
  motor1->run(FORWARD);
  motor1->setSpeed(sp+3);
  motor2->run(BACKWARD);
  motor2->setSpeed(sp);
  motor3->run(FORWARD);
  motor3->setSpeed(sp);
  motor4->run(BACKWARD);
  motor4->setSpeed(sp);
}
// motors 1 and 4 are inveresed directions
void move_left(int sp)
{
  motor1->run(BACKWARD);
  motor1->setSpeed(sp+3);
  motor2->run(BACKWARD);
  motor2->setSpeed(sp);
  motor3->run(FORWARD);
  motor3->setSpeed(sp);
  motor4->run(FORWARD);
  motor4->setSpeed(sp);
}
//motors 1 and 4 are inveresed
void move_right(int sp)
{
  motor1->run(FORWARD);
  motor1->setSpeed(sp+3);
  motor2->run(FORWARD);
  motor2->setSpeed(sp);
  motor3->run(BACKWARD);
  motor3->setSpeed(sp);
  motor4->run(BACKWARD);
  motor4->setSpeed(sp);
}
//motors 1 and 4 are inversed
void move_diag_right(int sp)
{
  motor1->run(RELEASE);
  motor1->setSpeed(sp+3);
  motor2->run(FORWARD);
  motor2->setSpeed(sp);
  motor3->run(BACKWARD);
  motor3->setSpeed(sp);
  motor4->run(RELEASE);
  motor4->setSpeed(sp);
}
//motors 1 and 4 are inversed
void move_diag_left(int sp)
{
  motor1->run(BACKWARD);
  motor1->setSpeed(sp+3);
  motor2->run(RELEASE);
  motor2->setSpeed(sp);
  motor3->run(RELEASE);
  motor3->setSpeed(sp);
  motor4->run(FORWARD);
  motor4->setSpeed(sp);  
}

void Stop(int sp)
{
  motor1->run(BACKWARD);
  motor1->setSpeed(0);
  motor2->run(FORWARD);
  motor2->setSpeed(0);
  motor3->run(BACKWARD);
  motor3->setSpeed(0);
  motor4->run(FORWARD);
  motor4->setSpeed(0);  
}
//motors 1 and 4 are inversed
void square_left(int sp)
{
  motor1->run(FORWARD);
  motor1->setSpeed(sp);
  motor2->run(FORWARD);
  motor2->setSpeed(sp);
  motor3->run(FORWARD);
  motor3->setSpeed(sp);
  motor4->run(FORWARD);
  motor4->setSpeed(sp); 
  
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
  motor4->setSpeed(sp); 
  
}


void move_backward_ramp()
{
  motor1->run(FORWARD);
  motor1->setSpeed(rsp);
  motor2->run(BACKWARD);
  motor2->setSpeed(rsp);
  motor3->run(FORWARD);
  motor3->setSpeed(rsp);
  motor4->run(BACKWARD);
  motor4->setSpeed(rsp);
}




