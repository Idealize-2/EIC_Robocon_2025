void initializeController() {
  // Serial.begin(115200);
  Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
  const uint8_t* addr = BP32.localBdAddress();
  Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

  // Setup the Bluepad32 callbacks
  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.forgetBluetoothKeys();
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
      //Serial.println("has data");
      if (myController->isGamepad()) {
        x_ctrl = mapf(myController->axisX(), -511, 512, -1, 1);
        y_ctrl = mapf(myController->axisY(), -511, 512, -1, 1);
        x_turn = mapf(myController->axisRX(), -511, 512, -1, 1);
        y_turn = mapf(myController->axisRY(), -511, 512, -1, 1);
      }
      //swap robot direction
      
      if (!myController->x() && Xstate) 
      {
          motor1.Swap();
          motor2.Swap();
          motor3.Swap();   
          motor4.Swap();
          delay( 10 );
          Serial.println("------------------swap-------------------");
      }
      Xstate = myController->x();
      

      if ( myController->a() && !Astate )
      {
        AllDelay.push_back( GlobalDelay( []() { Serial.println("this is fire!!");}
         ,[]() { Serial.println("End delay"); } 
         , 1000));
      }
      Astate = myController->a();

      if( myController->b() )
      {
          teleEncoder.setCount( 0 );
          teleEncoder.setFilter( 10 ); 
          // long start_operation = millis(); // idk why i put this but it cool 😎
          last_operation = millis();
          prev_count = teleEncoder.getCount(); //intialize prev_count   

          //lecate.run(-50);
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
          //lecate.run(-50);
          start_operation = millis();
          lecateDelay = true;
          isAutoDown = true;

        }

      }
      if ( myController->r1() )
      {
        // Serial.println("X pressd")

        feeder.run( 250);
        

      }
      if( myController->r2() )
      {
        Serial.println("------------OK------------------");
        if ( !isAutoAim )
        {
          isAutoAim = true;
        } 
      }
    }
  }
}