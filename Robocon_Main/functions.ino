void _initializePID(PID_v2* PID) {
  PID->SetOutputLimits(-MAX_RPM, MAX_RPM);
  PID->Start(0,   // input
             0,   // current output
             0);  // setpoint
  PID->SetSampleTime(10);
}

void initiateMotorsAndEncoders() {
  // Motors
  Serial.begin(115200);
  Wire.begin(MOTOR_SDA, MOTOR_SCL);

  // Set encoder pins as input
  pinMode(encoder1PinA, INPUT);
  pinMode(encoder1PinB, INPUT);
  pinMode(encoder2PinA, INPUT);
  pinMode(encoder2PinB, INPUT);
  pinMode(encoder3PinA, INPUT);
  pinMode(encoder3PinB, INPUT);
  pinMode(encoder4PinA, INPUT);
  pinMode(encoder4PinB, INPUT);

  //--------recently added-------//
  pinMode( teleEncoderPinA , INPUT );
  pinMode( teleEncoderPinB , INPUT );
  //-----------------------------//

  // Enable the weak pull up resistors
  ESP32Encoder::useInternalWeakPullResistors = puType::up;
  // Attach the encoder pins to the ESP32Encoder object
  encoder1.attachFullQuad(encoder1PinA, encoder1PinB);
  encoder2.attachFullQuad(encoder2PinB, encoder2PinA);
  encoder3.attachFullQuad(encoder3PinA, encoder3PinB);
  encoder4.attachFullQuad(encoder4PinB, encoder4PinA);

  //--------recently added-------//
  teleEncoder.attachFullQuad( teleEncoderPinA , teleEncoderPinB ); 
  //-----------------------------//

  // PIDs
  _initializePID(&PID_encoder1);
  _initializePID(&PID_encoder2);
  _initializePID(&PID_encoder3);
  _initializePID(&PID_encoder4);

  
}


void initializeController() {
  // Serial.begin(115200);
  Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
  const uint8_t* addr = BP32.localBdAddress();
  Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

  // Setup the Bluepad32 callbacks
  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.forgetBluetoothKeys();
}

void resetEncoders() {
  encoder1.setCount(0);
  encoder1.setFilter(10);
  encoder2.setCount(0);
  encoder2.setFilter(10);
  encoder3.setCount(0);
  encoder3.setFilter(10);
  encoder4.setCount(0);
  encoder4.setFilter(10);

  //--------recently added-------//
  teleEncoder.setCount( 0 );
  teleEncoder.setFilter( 10 );
  //-----------------------------//
}

void calculateRPMs() {
  // Calculate rpm1 every second
  long currentTime = millis();
  long deltaTime = currentTime - lastTime;
  if (deltaTime >= 10) {
    // Read encoder position
    long currentPosition = encoder1.getCount();
    long positionChange = currentPosition - lastCount1;
    rpm1 = ((positionChange * 60 * 1000 / deltaTime) / ppr);
    lastCount1 = currentPosition;

    currentPosition = encoder2.getCount();
    positionChange = currentPosition - lastCount2;
    rpm2 = ((positionChange * 60 * 1000 / deltaTime) / ppr);
    lastCount2 = currentPosition;

    currentPosition = encoder3.getCount();
    positionChange = currentPosition - lastCount3;
    rpm3 = ((positionChange * 60 * 1000 / deltaTime) / ppr);
    lastCount3 = currentPosition;

    currentPosition = encoder4.getCount();
    positionChange = currentPosition - lastCount4;
    rpm4 = ((positionChange * 60 * 1000 / deltaTime) / ppr);
    lastCount4 = currentPosition;

    lastTime = currentTime;
    // Serial.print(rpm1);
    // Serial.print(" ");
    // Serial.print(rpm2);
    // Serial.print(" ");
    // Serial.print(rpm3);
    // Serial.print(" ");
    // Serial.println(rpm4);
  }
}

void motor(int motorNum, float rpm) {
  // Speed is -100 to 100
  int pwm;
  // I2C Logic, ***look on the white board for details on the motor numbers***

  switch (motorNum) {
    case 1:
      // start Wire connection to the correct address (I cannot remember the code)
      Wire.beginTransmission(Left_I2C_ADDRESS);
      Wire.write(Front_I2C_ADDRESS);
      PID_encoder1.Setpoint(rpm);
      // pwm = RPMToPWM(PID_encoder1.Run(rpm1));
      // pwm = PID_encoder1.Run(rpm1);
      pwm = rpm;
      // pwm = RPMToPWM(rpm);
      // Set motor speed on the motor driver
      Serial.print("1:");
      Serial.print(rpm1);
      Serial.print(" ");
      Serial.print(rpm);
      Serial.print(" ");
      break;

    case 2:
      // start Wire connection to the correct address (I cannot remember the code)
      Wire.beginTransmission(Right_I2C_ADDRESS);
      Wire.write(Front_I2C_ADDRESS);
      PID_encoder2.Setpoint(rpm);
      // pwm = RPMToPWM(PID_encoder2.Run(rpm2));
      // pwm = PID_encoder2.Run(rpm2);
      pwm = rpm;
      // pwm = RPMToPWM(rpm);
      // Set motor speed on the motor driver
      Serial.print("2:");
      Serial.print(rpm2);
      Serial.print(" ");
      Serial.print(rpm);
      Serial.print(" ");
      break;

    case 3:
      // start Wire connection to the correct address (I cannot remember the code)
      Wire.beginTransmission(Left_I2C_ADDRESS);
      Wire.write(Back_I2C_ADDRESS);
      PID_encoder3.Setpoint(rpm);
      // pwm = RPMToPWM(PID_encoder3.Run(rpm3));
      // pwm = PID_encoder3.Run(rpm3);
      pwm = rpm;
      // pwm = RPMToPWM(rpm);
      // Set motor speed on the motor driver
      Serial.print("3:");
      Serial.print(rpm3);
      Serial.print(" ");
      Serial.print(rpm);
      Serial.print(" ");
      break;

    case 4:
      // start Wire connection to the correct address (I cannot remember the code)
      Wire.beginTransmission(Right_I2C_ADDRESS);
      Wire.write(Back_I2C_ADDRESS);
      PID_encoder4.Setpoint(rpm);
      // pwm = RPMToPWM(PID_encoder4.Run(rpm4));
      // pwm = PID_encoder4.Run(rpm4);
      pwm = rpm;
      // pwm = RPMToPWM(rpm);
      // Set motor speed on the motor driver
      Serial.print("4:");
      Serial.print(rpm4);
      Serial.print(" ");
      Serial.print(rpm);
      Serial.println(" ");
      break;

    case 5:
      Wire.beginTransmission(Lecate_I2C_ADDRESS);
      Wire.write(LeftLecatePin);
      pwm = rpm;
      // Set motor speed on the motor driver
      Serial.print("5:");
      Serial.print(pwm);
      Serial.println(' ');
      break;

    case 6:
      Wire.beginTransmission(Lecate_I2C_ADDRESS);
      Wire.write(RightLecatePin);
      pwm = rpm;
      
      Serial.print("6:");
      Serial.print(pwm);
      Serial.println(' ');
      break;

    case 7:
      Wire.beginTransmission(Gripper_I2C_ADDRESS);
      Wire.write(LeftGripperPin);
      pwm = rpm;

      Serial.print("7:");
      Serial.print(pwm);
      Serial.println(' ');
      break;

    case 8:
      Wire.beginTransmission(Gripper_I2C_ADDRESS);
      Wire.write(RightGripperPin);
      pwm = rpm;
      Serial.print("8:");
      Serial.print(pwm);
      Serial.println(' ');
      break;

    // case 9:
    //   Wire.beginTransmission(Telescopic_I2C_ADDRESS);
    //   Wire.write(LeftTelescopicPin);
    //   pwm = rpm;
    //   //PID_encoder4.Setpoint(rpm);
    //   // pwm = RPMToPWM(PID_encoder4.Run(rpm4));
    //   //pwm = -PID_encoder4.Run(rpm4);
    //   // pwm = RPMToPWM(rpm);
    //   // Set motor speed on the motor driver
    //   Serial.print("9:");
    //   Serial.print(pwm);
    //   Serial.println(' ');
    //   break;

    // case 10:
    //   Wire.beginTransmission(Telescopic_I2C_ADDRESS);
    //   Wire.write(RightTelescopicPin);
    //   pwm = rpm;
    //   //PID_encoder4.Setpoint(rpm);
    //   // pwm = RPMToPWM(PID_encoder4.Run(rpm4));
    //   //pwm = -PID_encoder4.Run(rpm4);
    //   // pwm = RPMToPWM(rpm);
    //   // Set motor speed on the motor driver
    //   Serial.print("10:");
    //   Serial.print(pwm);
    //   Serial.println(' ');
    //   break;


    default:
      // impossible cuz we only have 4 wheels
      break;
  }
  int dir = pwm > 0 ? 1 : 2;
  if (pwm == 0) { dir = 3; }
  Wire.write(dir);
  Wire.write(int(abs(pwm)));
  Wire.endTransmission();
}

void movebase(int rpm, float theta, int turn) {
  float sin_theta = sin(theta - M_PI / 4);
  float cos_theta = cos(theta - M_PI / 4);
  float max_value = max(abs(sin_theta), abs(cos_theta));
  int speed1 = rpm * cos_theta / max_value + turn;
  int speed2 = rpm * sin_theta / max_value + turn;
  int speed3 = rpm * sin_theta / max_value - turn;
  int speed4 = rpm * cos_theta / max_value - turn;

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

  motor(1, speed1);
  motor(2, -speed2);
  motor(3, -speed3);
  motor(4, speed4);
}

// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.
void onConnectedController(ControllerPtr ctl) {
  bool foundEmptySlot = false;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      Serial.printf("CALLBACK: Controller is connected, index=%d\n", i);
      // Additionally, you can get certain gamepad properties like:
      // Model, VID, PID, BTAddr, flags, etc.
      ControllerProperties properties = ctl->getProperties();
      Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n", ctl->getModelName().c_str(), properties.vendor_id,
                    properties.product_id);
      myControllers[i] = ctl;
      foundEmptySlot = true;
      break;
    }
  }
  if (!foundEmptySlot) {
    Serial.println("CALLBACK: Controller connected, but could not found empty slot");
  }
}

void onDisconnectedController(ControllerPtr ctl) {
  bool foundController = false;

  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      Serial.printf("CALLBACK: Controller disconnected from index=%d\n", i);
      myControllers[i] = nullptr;
      foundController = true;
      break;
    }
  }

  if (!foundController) {
    Serial.println("CALLBACK: Controller disconnected, but not found in myControllers");
  }
}

void processControllers() {
  for (auto myController : myControllers) {
    if (myController && myController->isConnected() && myController->hasData()) {
      //Serial.printf("has data");
      if (myController->isGamepad()) {
        x_ctrl = mapf(myController->axisX(), -511, 512, -1, 1);
        y_ctrl = mapf(myController->axisY(), -511, 512, -1, 1);
        x_turn = mapf(myController->axisRX(), -511, 512, -1, 1);
        y_turn = mapf(myController->axisRY(), -511, 512, -1, 1);
      }
      // if(myController->x())
      // {
      //   Xpressed = !Xpressed;
      //   if(Xpressed)
      //   {
      //     motor(5,-255);
      //     motor(6,-255);
      //     delay(1200);
      //   }
      //   else
      //   {
      //     motor(5,200);
      //     motor(6,200);
      //     delay(200);
      //     motor(5,150);
      //     motor(6,150);
      //     delay(200);
      //   }
      // }
      // else
      // {
      //   motor(5,0);
      //   motor(6,0);
      //   delay(500);
      // }]
      if (myController->x()) {
        motor(8, 250);
        delay(2000);
        motor(8, 0);
        delay(2000);
      }

      if (myController->a()) {
        motor(7, 180);
        motor(8, -180);
        delay(1200);

        double prevt = millis();
        double currentt = millis();
        while (currentt - prevt < 13500) {
          motor(7, 180);
          motor(8, -180);
          // motor(9, 225);
          // motor(10, 225);
          currentt = millis();
        }
        delay(1000);

        motor(7, -100);
        motor(8, 100);
        delay(50);

        motor(5, 255);
        motor(6, 255);



        prevt = millis();
        currentt = millis();
        while (currentt - prevt < 1000) {
          motor(1, 120);
          motor(2, 120);
          motor(3, 120);
          motor(4, 120);
          currentt = millis();
        }
        motor(1, 0);
        motor(2, 0);
        motor(3, 0);
        motor(4, 0);
        delay(1200);
        motor(7, 0);
        motor(8, 0);
      }
      //TeleAutoUp
      if( myController->b() )
      {
          teleEncoder.setCount( 0 );
          teleEncoder.setFilter( 10 ); 
          // long start_operation = millis(); // idk why i put this but it cool 😎
          last_operation = millis();
          prev_count = teleEncoder.getCount(); //intialize prev_count   

          motor(5,-100);
          motor(6,-100);
          start_operation = millis();
          lecateDelay = true;

          Serial.println("Strart Teleing");
          isAutoDown = false;
          isAutoUp = true;
          //TeleUp( 250 );
      }
      //TeleAutoDown
      if ( myController->y() ) {
        // motor(9, -185);
        // motor(10, -185);
        if( !isAutoDown )
        {
          Serial.println("----satrt deceaseing-----");
          motor(5,-100);
          motor(6,-100);
          start_operation = millis();
          lecateDelay = true;
          isAutoDown = true;

        }

      }
    }
  }
}