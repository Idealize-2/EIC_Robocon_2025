void initiateMotorsAndEncoders() {
  // Motors
  Serial.begin(115200);
  Wire.begin(MOTOR_SDA, MOTOR_SCL);

  //--------recently added-------//
  // pinMode( teleEncoderPinA , INPUT );
  // pinMode( teleEncoderPinB , INPUT );
  //-----------------------------//

  // Enable the weak pull up resistors
  // ESP32Encoder::useInternalWeakPullResistors = puType::up;

  //--------recently added-------//
  // teleEncoder.attachFullQuad( teleEncoderPinA , teleEncoderPinB ); 
  //-----------------------------//
}



void resetEncoders() {
  //--------recently added-------//
  teleEncoder.setCount( 0 );
  teleEncoder.setFilter( 10 );
  //-----------------------------//
}

void movebase(int rpm, float theta, int turn) {
  float sin_theta = sin(theta - M_PI / 4);
  float cos_theta = cos(theta - M_PI / 4);
  float max_value = max(abs(sin_theta), abs(cos_theta));
  int speed1 , speed2 , speed3 , speed4;
  if(!motor1.dir())
  {
    speed1 = rpm * cos_theta / max_value + turn;
    speed2 = rpm * sin_theta / max_value + turn;
    speed3 = rpm * sin_theta / max_value - turn;
    speed4 = rpm * cos_theta / max_value - turn;
  }
  else
  {
    speed1 = rpm * cos_theta / max_value - turn;
    speed2 = rpm * sin_theta / max_value - turn;
    speed3 = rpm * sin_theta / max_value + turn;
    speed4 = rpm * cos_theta / max_value + turn;
  }
  

  

  // if ((rpm + abs(turn)) > 1){
  //   rpm1 /= rpm + turn;
  //   rpm2 /= rpm + turn;
  //   rpm3 /= rpm + turn;
  //   rpm4 /= rpm + turn;
  // }

  // Serial.print(theta);
  // Serial.print(" ");
  // Serial.print(sin_theta);
  // Serial.print(" ");
  // Serial.println(cos_theta);

  //   Serial.print(" ");
  //   Serial.print(speed1);
  //   Serial.print(" ");
  //   Serial.print(speed2);
  //   Serial.print(" ");
  //   Serial.print(speed3);
  //   Serial.print(" ");
  //   Serial.println(speed4);

  // Serial.print(sin(theta - M_PI / 4));
  // Serial.print(" ");
  // Serial.println(theta - M_PI / 4);

  //Serial.println("run");

  motor1.run(speed1);
  motor2.run(-speed2);
  motor3.run(-speed3);
  motor4.run(speed4);
}