
 void main_movement() 
 {
  int distance_Front_0 = distance_Front();
  int Moving_average_Rear_0 = Moving_average_Rear();
  int distance_Right_0 = distance_Right();  
    
    //Move straight when nothing is in the way.
  if(distance_Front_0 >= 500)
    {      move_forward(ssp);
      Serial.println(distance_Front_0);
      Serial.println("Moving forward");
      ///delay(1);
    }
  else if(distance_Right() < 500)
    {
       move_backward(ssp);
       Serial.println("Moving backward");
       delay(100);
     }
    else if(Moving_average_Rear() < 500)
     {
          move_right(ssp);
          Serial.println("Moving right");
          delay(500);
      }
 }

int Centering()
{
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
     if( Moving_average_Rear() > 135)
     {
          move_backward(sssp);
     }
     else if (Moving_average_Rear() < 128) 
     {
          move_forward(sssp);
     }
     else if (Moving_average_Rear() >=128 || Moving_average_Rear() <=135)
     {
         Stop(ssp);
// delay(1000);
         x=2;
     }
 }
}
////////test

int Centering2()
{
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
/*
int Centering2()
{
  int x=0;
  while( x==0)
  {
    if (distance_Right() < 450)
    {
      move_left(ssp);
      Serial.println("moving left");
    }
    else if (distance_Left() < 450)
    {
      move_right(ssp);
      Serial.println("moving right");
    }
    else if (distance_Right() >= 450)
    {
      Serial.println("stop");
      Stop(ssp);
      delay(2000);
      x=2;
    }
  }
 */
 /*
 while (x==1)
 {
 if( Moving_average_Rear()() > 135)
 {
  move_backward(ssp);
 }
 else if (Moving_average_Rear()() < 125) 
 {
  move_forward(ssp);
 }
 else if (Moving_average_Rear()() <=125 || Moving_average_Rear()() >=135){
 Stop(ssp);
// delay(1000);
 x=2;
  }
 }
*/
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
   
    //Serial.println("checking angle");
    int y = 0;
    
    while (y ==0)
    {
      MPU_loop();
      if (kalAngleX < 7)
        {
          Serial.println("starting to move up ramp");
          Serial.println(kalAngleX);
          MPU_loop();
          move_backward(brsp);  
        }
      
       else if (kalAngleX >= 7)
        {
          Serial.println("Moving up the ramp");
          move_backward_ramp(rsp);
          MPU_loop();
          if (distance_Rear() <= 200)
          {
            Stop(sp);
            move_backward(sssp);
            Stop(sp);
            y = 1;     
          }   
         }
      }
     while (y == 1)
     {
      //Centering();
      y == 2;
     }
    
}


/////////////////////////////////////
/////////////IR code reading/////////
////////////////////////////////////
void codemoveA(){
  
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
  int x =0;
  int y =0;
  while(x==0)
  {
    if (B==0)
    {
      if (distance_Left() > 260)
      {
      move_diag_left(sp); 
      }
      else if (distance_Left() <= 260 )
      {
      Stop(sp);
      x=1;
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
      x=1;
    }
   }
  }    
}
  
 
void codemoveC(){
  
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
      chest_exit = 1;
     } 
     
  }    
}

void flagmove()
{
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
  motor3->run(FORWARD);
  motor3->setSpeed(sp);
  motor4->run(BACKWARD);
  motor4->setSpeed(sp);
}
// motors 1 and 4 are inverse driections
void move_backward(int sp)
{
  motor1->run(FORWARD);
  motor1->setSpeed(sp+3);
  motor2->run(BACKWARD);
  motor2->setSpeed(sp);
  motor3->run(BACKWARD);
  motor3->setSpeed(sp);
  motor4->run(FORWARD);
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
  motor3->run(RELEASE);
  motor3->setSpeed(sp);
  motor4->run(BACKWARD);
  motor4->setSpeed(sp);
}
//motors 1 and 4 are inversed
void move_diag_left(int sp)
{
  motor1->run(BACKWARD);
  motor1->setSpeed(sp+3);
  motor2->run(RELEASE);
  motor2->setSpeed(sp);
  motor3->run(FORWARD);
  motor3->setSpeed(sp);
  motor4->run(RELEASE);
  motor4->setSpeed(sp);  
}

void Stop(int sp)
{
  motor1->run(RELEASE);
  motor1->setSpeed(sp);
  motor2->run(RELEASE);
  motor2->setSpeed(sp);
  motor3->run(RELEASE);
  motor3->setSpeed(sp);
  motor4->run(RELEASE);
  motor4->setSpeed(sp);  
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


void move_backward_ramp(int sp)
{
  motor1->run(FORWARD);
  motor1->setSpeed(sp+3);
  motor2->run(BACKWARD);
  motor2->setSpeed(sp);
  motor3->run(BACKWARD);
  motor3->setSpeed(fsp);
  motor4->run(FORWARD);
  motor4->setSpeed(fsp);
}



