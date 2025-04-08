

// highest height that telescope can go


void TeleUp( int pwmSpeed )
{
  //up
  //Serial.println("LEt gooo");
  digitalWrite( TelePinA , HIGH);
  digitalWrite( TelePinB , LOW);
  analogWrite( TelePWM , pwmSpeed );
}

void TeleUp( int pwmSpeed , int time )
{
  digitalWrite( TelePinA , HIGH );
  digitalWrite( TelePinB , LOW );
  analogWrite( TelePWM , pwmSpeed );

  delay( time );
  TeleStop( );
}


// 
void TeleDown( int pwmSpeed )
{
  isTeleStall = false;
  digitalWrite( TelePinA , LOW );
  digitalWrite( TelePinB , HIGH );
  analogWrite( TelePWM , pwmSpeed );
}

void TeleAutoDown( )
{
    if( teleEncoder.getCount() < 2000 )
    {
      //canLowerPeak = false;
      isTeleStall = false;
      isAutoUp = false;
      isAutoDown = false;
      peakStall = 0;
      TeleUp ( 0 );
      //TeleStop();
      Serial.println("----- End Auto Down -----");
    }
    // if( canLowerPeak )
    // {
    //   peakStall -= 100 ;
    //   canLowerPeak = false;
    //   Serial.println(" -----------set----------- ");
    // }
}

bool canTimeStamp = true;
const long DECIDE_PEAK = 600; // 0.6 sec
const long DECIDE_STOP = 500; // encoder step to decide it not going more than this
long last_operation;
long prev_count;
long start_operation;

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
  TeleUp( 250 );
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
      peakStall = teleEncoder.getCount()-2000;
      TeleUp( 0 );
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
int i = 0;

void TeleStall(  )
{
 // Serial.println(  peakStall - teleEncoder.getCount() );
  if( lecateDelay )
  {
       if( (( millis() - start_operation ) > 2000))
      {
        motor(5,0);
        motor(6,0);
        lecateDelay = false;
      }
  }
 
  if( isAutoDown )
  {
    TeleUp( 30 );
    Serial.println(" deceaseing ");
  }
  else
  {
    
    if( (peakStall - teleEncoder.getCount() ) < 0  )
    {
      Serial.println(" Tooooo high ");
      TeleUp( 25 );
    }
    else if( abs( peakStall - teleEncoder.getCount() ) < Stall_bypass) 
    {
      Serial.println(" PEak  PEak Peak");
      TeleUp( 175 );
    }
    else
    {
      Serial.println(  peakStall - teleEncoder.getCount()  );
      Serial.println(" Up Up Up");
      TeleUp( 210 );
    }
  }
  
  // int cal_diff = abs( peakStall - teleEncoder.getCount() );
  // if( cal_diff > 2500 )
  // {
  //   Serial.println(" Too high");
  //   TeleUp( 250 );
  // }
  // else if( cal_diff < Stall_bypass)
  // {
  //   Serial.println(" Peak Peak Peak ");
  //   TeleUp( 175 );
  // }
  // else 
  // {
  //   Serial.println( cal_diff / 10);
  //   // TeleUp( cal_diff  / 10 );
  //   TeleUp( 210 );
  // }
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