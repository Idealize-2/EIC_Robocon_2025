
// ///////////////////////////////////////////////          BruteForce           /////////////////////////////////////////////



void BFver(int x) {
  //Serial.println( x );
  // int xmap = map(abs(x), 10, 640, 19, 40);
  int xmap = PID_camera_x.Run(x);
  //  if(x > 5)
  //  {
  //    //Serial.print("////////////////////////////////Right/////////////////////////////////");
  //   //  Serial.print("left move");
  //   //  Serial.print(-xmap);
  //   //  Serial.print("Right move");
  //   //  Serial.println(xmap);

  //    motor1.run( xmap );
  //    motor2.run( -xmap );
  //    motor3.run( xmap );
  //    motor4.run( -xmap );

  //  }
  //  else if(x < -5)
  //  {
  //    //Serial.print("////////////////////////////////Left///////////////////////////////////");
  //   //  Serial.print("left move");
  //   //  Serial.print(xmap);
  //   //  Serial.print("Right move");
  //   //  Serial.println(-xmap);

  //    motor1.run( -xmap );
  //    motor2.run( xmap );
  //    motor3.run( -xmap );
  //    motor4.run( xmap );
  //  }
  if (abs(x) > 5) {
    motor1.run(-xmap);
    motor2.run(xmap);
    motor3.run(-xmap);
    motor4.run(xmap);
  } else {
    //Serial.print("//////////////////////////////Center///////////////////////////////////");
    isAutoAim = false;
    motor1.run(0);
    motor2.run(0);
    motor3.run(0);
    motor4.run(0);
  }
}
// ///////////////////////////////////////////////////////         PID            ////////////////////////////////////
// const double kp = 1 ,ki = 1 , kd = 1;

// PID_v2 PID_cv(kp, ki, dd, PID::Direct);

// const turnSpeed = 120;
// void PIDver( int x)
// {

// }
