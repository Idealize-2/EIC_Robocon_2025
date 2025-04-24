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
    // Serial.println( canRumble );
    // if( canRumble && myController && myController->isConnected() ){
    //      myController->playDualRumble(0 /* delayedStartMs */, 250 /* durationMs */, 0x80 /* weakMagnitude */,
    //                         0x40 /* strongMagnitude */);
    //      Serial.println("Rumbleing");
    // } 
    
    if (myController && myController->isConnected() && myController->hasData()) {

      //Serial.println("has data");
      if (myController->isGamepad()) {
        x_ctrl = mapf(myController->axisX(), -511, 512, -1, 1);
        y_ctrl = mapf(myController->axisY(), -511, 512, -1, 1);
        x_turn = mapf(myController->axisRX(), -511, 512, -1, 1);
        y_turn = mapf(myController->axisRY(), -511, 512, -1, 1);
      }
  
//--------------------------------------------------------------- MISC BUTTON ----------------------------------------------------------------------//

      if( myController->miscHome() ){
        Serial.println("LEft");
      }
      
      if( myController->miscCapture() && !miscCenterState ){
        Serial.println("Center");
        motor1.Swap();
        motor2.Swap();
        motor3.Swap();   
        motor4.Swap();
        delay( 10 );
        Serial.println("------------------swap-------------------");
      }
      miscCenterState = myController->miscCapture();

      if( myController->miscSelect() ){
        Serial.println("Right");
      }

//--------------------------------------------------------------- MISC BUTTON ----------------------------------------------------------------------//


//------------------------------------------------------------------- D PAD --------------------------------------------------------------------//

      if( myController->dpad() == 8 ) 
      {
          Serial.println("IN");
          Lecate.run( 180 );
      }
      if( !myController->dpad() == 8 && butLeftState )
      {
        Lecate.run( 0 );
      }
      butLeftState = myController->dpad() == 8;


      if( myController->dpad() == 4 ) 
      {
          Serial.println("Out");
          Lecate.run( -180 );
      }
      if( !myController->dpad() == 4 && butRightState  )
      {
         Lecate.run( 0 );
      }
      butRightState = myController->dpad() == 4;



      if( myController->dpad() == 1 ) 
      {
        Tele.run( 255 );
        Lecate.run( -180 );
        Serial.println( teleEncoder.getCount() );
      }
      if( !myController->dpad() == 1 && butUpState)
      {
        Lecate.run( 0 );
        peakStall = teleEncoder.getCount();
        Tele.stop();
        isTeleStall = true;
        isAutoUp = false;
      }
      butUpState = myController->dpad() == 1;


      if( myController->dpad() == 2 ) 
      {
        Tele.run( 90 );
        Lecate.run( -180 );
        Serial.println( teleEncoder.getCount() );
      }
      if( !myController->dpad() == 2 && butDownState ) 
      {
        Lecate.run( 0 );
        peakStall = teleEncoder.getCount();
        Tele.stop();
        isTeleStall = true;
        isAutoUp = false;
      }
      butDownState = myController->dpad() == 2;
//------------------------------------------------------------------- D PAD --------------------------------------------------------------------//


//------------------------------------------------------------------- X A Y B -------------------------------------------------------------------//

      //swap robot direction
      if (!myController->x() && XState) 
      {
        isKeep = !isKeep;
      }
      XState = myController->x();      

      if ( myController->a() && !AState )
      {
        AllDelay.push_back( GlobalDelay( []() { Serial.println("this is fire!!");}
         ,[]() { Serial.println("End delay"); } 
         , 1000));
        //Serial.println("x");
      }
      AState = myController->a();

      if( myController->b() && !BState )
      {
          
      }
      BState = myController->b();

      //TeleAutoDown
      if ( myController->y() && !YState ) {
        // motor(9, -185);
        // motor(10, -185);
        if( isFunctionActivate )
        {
          isFunctionActivate = false;
          AllDelay.push_back(
            GlobalDelay(
              []() {Lecate.run( -150 );},
              []() {pressAutoDown();},
              700
            )
          );
        }
        else
        {
          pressAutoDown();
        }

      }
      YState = myController->y();

//------------------------------------------------------------------- X A Y B -------------------------------------------------------------------//

//------------------------------------------------------------------- L1 L2 LTHUMP ------------------------------------------------------------------//
      if ( myController->l1() && !L1State )
      {
        autoPoiBall();
      }
      L1State = myController->l1();

      if ( myController->l2() && !L2State )
      {
        shootON = !shootON;
        if( shootON ){
          Shooter.run( 255 );
        }
        else
        {
          Shooter.run( 0 );
        }
      }
      L2State = myController->l2();

      if ( myController->thumbL() )
      {
        Serial.println("l3 pressd");
      }

//------------------------------------------------------------------- L1 L2 L THUMP ------------------------------------------------------------------//

 //------------------------------------------------------------------- R1 R2 R THUMP ------------------------------------------------------------------//
   
      if ( myController->r1() && !R1State )
      {
        //feed ball to shooter
        shoot();
      }
      R1State = myController->r1();


      if( myController->r2() && !R2State )
      {
        Serial.print("-------------------------------Hello-------------------------------");
        
        AllDelay.push_back( GlobalDelay([]() {
          if( Serial.available() )
          {
            //x = Serial.readString().toInt();
            String input = Serial.readStringUntil('\n');
            x = input.toInt();
            BFver( x );
            //Serial.println( x );
          }
        } ,
        []() {Serial.println("End Auto Aim");}, 5000));
      }
      R2State = myController->r2();

      if ( myController->thumbR() )
      {
        Serial.println("r3 pressd");
      }

 //------------------------------------------------------------------- R1 R2 R THUMP ------------------------------------------------------------------//
    }
  }
}






void pressAutoDown()
{
  Serial.println("----satrt deceaseing-----");
  if( !isAutoDown )
  {
    Serial.println("----satrt deceaseing-----");
    start_operation = millis();
    isAutoUp = false;
    isAutoDown = true;
  }
}
void pressAutoUp()
{
  if(!isOnTop)
  {
    teleEncoder.setCount( 0 );
    teleEncoder.setFilter( 10 ); 
    // long start_operation = millis(); // idk why i put this but it cool 😎
    last_operation = millis();
    prev_count = teleEncoder.getCount(); //intialize prev_count   

    //lecate.run(-50);
    start_operation = millis();

    Serial.println("Strart Teleing");
    isAutoDown = false;
    isAutoUp = true;
    //TeleUp( 250 );
  }
  
}

void shoot()
{
  AllDelay.push_back( 
      GlobalDelay( []() {feeder.run( 100 ); },
                    []() { AllDelay.push_back(
                            GlobalDelay(
                              []() {feeder.run( 250 );},
                              []() {AllDelay.push_back(
                                      GlobalDelay(
                                        []() {feeder.run( -50 );},
                                        []() {feeder.run( 0 );},
                                        300
                                      ));},
                              300
                            )
                    );},
        600         
      )
    );
}

void autoPoiBall(){
  //Poi
  isKeep = false;
  //Delay
  AllDelay.push_back(
    GlobalDelay(
      []() {},
      []() {
        AllDelay.push_back(
            GlobalDelay(
              []() {
                Lecate.run(150);
                onMoveExecute = true;
                motor1.run(50);
                motor2.run(50);
                motor3.run(50);
                motor4.run(50);
              },
              []() {
                isFunctionActivate = true;
                onMoveExecute = false;
                motor1.run(0);
                motor2.run(0);
                motor3.run(0);
                motor4.run(0);
              },
              500
            )
        );
      },
      1000
    )
  );
}
