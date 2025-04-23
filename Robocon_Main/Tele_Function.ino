

void TeleAutoDown( )
{
    if( teleEncoder.getCount() < 2000 )
    {
      //canLowerPeak = false;
      isTeleStall = false;
      isAutoUp = false;
      isAutoDown = false;
      peakStall = 0;
      Lecate.run( 0 );
      TeleStop();
      Serial.println("----- End Auto Down -----");
    }
}

void TeleAutoUp()
{
  Tele.run( 255 );
  Lecate.run( -180 );
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
      Lecate.run( 0 );
      peakStall = teleEncoder.getCount()-600;
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
  //       Lecate.run( 0 );
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
    Lecate.run( -180 );
    Serial.print( downPwm );
    Serial.println(" deceaseing ");
  }
  else
  {
    
    if( (peakStall - teleEncoder.getCount() ) < 0  )
    {
      Serial.println(" Tooooo high ");
      Tele.run( 50 );
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
