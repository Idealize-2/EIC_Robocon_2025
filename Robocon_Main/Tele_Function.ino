

void TeleAutoDown( )
{
    if( teleEncoder.getCount() < 2000 )
    {
      //canLowerPeak = false;
      isTeleStall = false;
      isAutoUp = false;
      isAutoDown = false;
      peakStall = 0;
      TeleStop();
      Serial.println("----- End Auto Down -----");
    }
    // if( canLowerPeak )
    // {
    //   peakStall -= 100 ;
    //   canLowerPeak = false;
    //   Serial.println(" -----------set----------- ");
    // }
}

void TeleAutoUp()
{
  // Need to put this v in controller to intialize
  // const long DECIDE_PEAK = 600; // 0.6 sec
  // const long DECIDE_STOP = 500; // encoder step to decide it not going more than this
  // // long start_operation = millis(); // idk why i put this but it cool 😎
  // long last_operation = millis();
  // long prev_count = teleEncoder.getCount(); //intialize prev_count

  // Serial.println("Strart Teleing");
  //while( isTeleAuto )
  Tele.run( 255 );
  Serial.println( teleEncoder.getCount() );
  if( teleEncoder.getCount() - prev_count > DECIDE_STOP )
  {
    prev_count =  teleEncoder.getCount();
    canTimeStamp = true;
    last_operation = millis();
    Serial.println("Cycle!");
  }
  else
  {
    if( canTimeStamp )
    {
      Serial.println("NEWSTAMP{E");
        last_operation = millis();
        canTimeStamp = false;
    }
    if ( millis() - last_operation > DECIDE_PEAK ) 
    {
      Serial.println("END---------------------------------------------");
      peakStall = teleEncoder.getCount()-1500;
      Tele.stop();
      isTeleStall = true;
      isAutoUp = false;
    }
    
  }
  // if( emergency_button )
  // {
  //   TeleStop();
  //   break;
  // }

}

const int Stall_bypass = 800;
int downPwm = 50;

void TeleStall(  )
{
 // Serial.println(  peakStall - teleEncoder.getCount() );
  // if( lecateDelay )
  // {
  //      if( (( millis() - start_operation ) > 1500))
  //     {
  //       motor(5,0);
  //       motor(6,0);
  //       lecateDelay = false;
  //     }
  // }
 
  if( isAutoDown )
  {
    if( ( millis() - last_operation ) > 100 )
    {
      long count_diff = prev_count - teleEncoder.getCount();
      downPwm = map( count_diff , 0 , 2000 , 35 , 100);
      //count_diff = teleEncoder.getCount();
      last_operation = millis();
      prev_count = teleEncoder.getCount();
    }
    Tele.run( downPwm );
    Serial.print( downPwm );
    Serial.println(" deceaseing ");
  }
  else
  {
    
    if( (peakStall - teleEncoder.getCount() ) < 0  )
    {
      Serial.println(" Tooooo high ");
      Tele.run( 40 );
    }
    else if( abs( peakStall - teleEncoder.getCount() ) < Stall_bypass) 
    {
      Serial.println(" PEak  PEak Peak");
      Tele.run( 180 );
    }
    else
    {
      Serial.println(  peakStall - teleEncoder.getCount()  );
      Serial.println(" Up Up Up");
      Tele.run( 210 );
    }
  }
}

void TeleStop()
{
  digitalWrite( TelePinA , LOW );
  digitalWrite( TelePinB , LOW );
  analogWrite( TelePWM , 0 );
  isTeleStall = false;
}



const float PEAKHIGH = 0.97; // 97 cm Highest telescope that can reach
long calEncode(float y)
{
  y /= 100;
  return PEAKHIGH / ( y * peakStall );
} 

// void runCM( int pwmSpeed , float cm , bool dir ) // y = ideal cm to reach
// {

//     if( dir )
//     {
//         TeleUp( pwmSpeed );
//         target_count = teleEncoder.getCount() + calEncode( cm ); 
//         while( teleEncoder.getCount() <= target_count ){};
//         TeleStop();
//     }
//     else
//     {
//       TeleDown( pwmSpeed );
//       target_count = teleEncoder.getCount() - calEncode( cm ); 
//       while( teleEncoder.getCount() >= target_count ){};
//       TeleStop();
//     }
// }
