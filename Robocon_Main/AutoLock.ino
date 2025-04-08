bool isAutoLock false;

///////////////////////////////////////////////          BruteForce           /////////////////////////////////////////////

void BFver( int x )
{
  int xmap = map(abs(x),0,640,0,245);
   if(x > 10)
   {
     Serial.print("////////////////////////////////Right/////////////////////////////////");
     Serial.print("left move");
     Serial.print(-xmap);
     Serial.print("Right move");
     Serial.println(xmap);

     // motor(1,50);
     // motor(2,-50);
     // motor(3,50);
     // motor(4,-50);

   }
   else if(x < -10)
   {
     Serial.print("////////////////////////////////Left///////////////////////////////////");
     Serial.print("left move");
     Serial.print(xmap);
     Serial.print("Right move");
     Serial.println(-xmap);

     // motor(1,-50);
     // motor(2,50);
     // motor(3,-50);
     // motor(4,50);
   }
   else
   {
     Serial.print("//////////////////////////////Center///////////////////////////////////");
     // motor(1, 0);
     // motor(2, 0);
     // motor(3, 0);
     // motor(4, 0);
   }
}
///////////////////////////////////////////////////////         PID            ////////////////////////////////////
const double kp = 1 ,ki = 1 , kd = 1;

PID_v2 PID_cv(kp, ki, dd, PID::Direct);

const turnSpeed = 120;
void PIDver( int x)
{
  
}
