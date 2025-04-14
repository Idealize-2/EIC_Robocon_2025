
// ///////////////////////////////////////////////          BruteForce           /////////////////////////////////////////////

void BFver( int x )
{
  //Serial.println( x );
  int xmap = map(abs(x),15,640,17,30);
   if(x > 10)
   {
     //Serial.print("////////////////////////////////Right/////////////////////////////////");
    //  Serial.print("left move");
    //  Serial.print(-xmap);
    //  Serial.print("Right move");
    //  Serial.println(xmap);

     motor(1,xmap);
     motor(2,-xmap);
     motor(3,xmap);
     motor(4,-xmap);

   }
   else if(x < -10)
   {
     //Serial.print("////////////////////////////////Left///////////////////////////////////");
    //  Serial.print("left move");
    //  Serial.print(xmap);
    //  Serial.print("Right move");
    //  Serial.println(-xmap);

     motor(1,-xmap);
     motor(2,xmap);
     motor(3,-xmap);
     motor(4,xmap);
   }
   else
   {
     //Serial.print("//////////////////////////////Center///////////////////////////////////");
     isAutoAim = false;
     motor(1, 0);
     motor(2, 0);
     motor(3, 0);
     motor(4, 0);
    //  if(x > 10 || x < -10)
    //  {
    //   isAutoAim = true;
    //  }
   }
}
// ///////////////////////////////////////////////////////         PID            ////////////////////////////////////
// const double kp = 1 ,ki = 1 , kd = 1;

// PID_v2 PID_cv(kp, ki, dd, PID::Direct);

// const turnSpeed = 120;
// void PIDver( int x)
// {
  
// }
