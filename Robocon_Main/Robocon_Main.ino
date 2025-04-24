#include <Arduino.h>
#include <ESP32Encoder.h>
#include <Wire.h>
#include <PID_v2.h>
#include <math.h>
#include <Bluepad32.h>
#include "Mapf.h"
#include "Motor.h"
#include "GlobalDelay.h"


// Include the list library
#include <list>


//DriveBase Address
#define Left_I2C_ADDRESS 0x50
#define Right_I2C_ADDRESS 0x57
#define Front_I2C_ADDRESS 0x85
#define Back_I2C_ADDRESS 0x86
// DriveBase Define
MotorI2C motor1( Left_I2C_ADDRESS , Front_I2C_ADDRESS ); // Front Left
MotorI2C motor2( Right_I2C_ADDRESS , Front_I2C_ADDRESS ); // Front Right
MotorI2C motor3( Left_I2C_ADDRESS , Back_I2C_ADDRESS ); // Back Left
MotorI2C motor4( Right_I2C_ADDRESS , Back_I2C_ADDRESS ); // Back Right

//Smile Drive communication
#define MOTOR_SDA 21
#define MOTOR_SCL 22
#define MAX_RPM 500

/*--------------------------------------------------Controller State Button config------------------------------------------------------------------*/
bool XState = false;
bool YState = false;
bool AState = false;
bool BState = false;

bool L1State = false;
bool L2State = false;
bool R1State = false;
bool R2State = false;

bool miscSystemState = false;
bool miscCenterState = false;
bool miscBackState = false;

bool butUpState = false;
bool butDownState = false;
bool butLeftState = false;
bool butRightState = false;
/*--------------------------------------------------Controller State Button config------------------------------------------------------------------*/



//----------------------------------------------------------- other state -------------------------------------------------------------------//
bool shootON = false;

bool lecateOn = false;

bool isOnTop = false;

//moveBaseOn
bool onMoveExecute = false;

//isfunctionActivate
bool isFunctionActivate = false;
//----------------------------------------------------------- other state -------------------------------------------------------------------//



/*---------Gripper---------*/
#define Gripper_I2C_ADDRESS 0x56
#define LeftGripperPin 0x86
#define RightGripperPin 0x85
//Gripper Define
MotorI2C Grip7(Gripper_I2C_ADDRESS , LeftGripperPin );
MotorI2C Grip8(Gripper_I2C_ADDRESS , RightGripperPin );

bool isKeep = false;

/*----------/Telescopic------------*/
#define TelePinA 26
#define TelePinB 25
#define TelePWM 27
MotorPIN Tele( TelePinA , TelePinB , TelePWM );

/*-----Feeder----*/
#define Feeder_I2C_ADDRESS 0x51
#define feederPin 0x85
MotorI2C feeder( Feeder_I2C_ADDRESS , feederPin , true);


//--------Tele Check etc. variable -----------//
bool isTeleStall = false;
bool isAutoUp = false;
bool isAutoDown = false;

//------for auto
bool canTimeStamp = true;
const long DECIDE_PEAK = 600; // 0.6 sec
const long DECIDE_STOP = 500; // encoder step to decide it not going more than this
long last_operation = millis();
long prev_count = 0;
long start_operation = millis();
//-----------------

long peakStall = 0;

//saddwadaw
bool isAutoAim = false;

// GlobalDelay Call
//allGlovalDelay.push( );
std::list<GlobalDelay> AllDelay;


// Shooter EV1 Ev2 PIn
#define EV12 16
#define EV12pwm 4
#define free 15
MotorPIN Shooter( EV12 , free ,EV12pwm );

// NEw Ev4 LEcate
#define E4inA 18 
#define E4inB 19
#define E4pwm 17
MotorPIN Lecate( E4inB , E4inA , E4pwm );




//---------------Telescope encoder
ESP32Encoder teleEncoder ;
//---------------------encoder Pin
#define teleEncoderPinA 14
#define teleEncoderPinB 13



/* ---------------- CONTROLLER INIT ---------------- */

ControllerPtr myControllers[BP32_MAX_GAMEPADS];
float x_ctrl = 0;
float y_ctrl = 0;
float x_turn = 0;
float y_turn = 0;


// Variables for rpm calculation
long lastCount1 = 0;
int rpm1 = 0;
long lastCount2 = 0;
int rpm2 = 0;
long lastCount3 = 0;
int rpm3 = 0;
long lastCount4 = 0;
int rpm4 = 0;

long lastTime = 0;
int ppr = 2048;

//------------------------------------ Instance PID shooter ------------------------------------//

PID_v2 PID_camera_x(0.2, 0, 0, PID::Direct);

float RPMToPWM(float rpm) {
  return abs(rpm / MAX_RPM * 255);
}

/* ---------------- MOTORS INIT ---------------- */
float speedFraction = 1;
int maxSpeed = 255;

/* ---------------- PySerial ---------------- */
int x;

/* ---------------- START PROGRAM ---------------- */

void setup() {
  Serial.begin(115200);
  initiateMotorsAndEncoders();
  initializeController();

  // penrai wa bug reset encoder;
  resetEncoders();

	Serial.setTimeout(1);

  PID_camera_x.Start(0, 0, 0);
  PID_camera_x.SetOutputLimits(-50, 50);

  //Serial.setTimeout(1);

  // /* ---------------- TEST ---------------- */
  // Serial.println("Starting...");
  // delay(6000);
  // Serial.println("Started!!!");

  
}

void loop() {
  //while (!Serial.available());

  //Serial.println( "Loop available" ) // for check loop

  //calculateRPMs();
  bool dataUpdated = BP32.update();
  if (dataUpdated) {
    //Serial.print("data updated");
    processControllers();
  }

  //Serial.println( teleEncoder.getCount() );
  // TeleUp(200,500);
  //Serial.println( AllDelay.size() );
  for (auto it = AllDelay.begin() ; it != AllDelay.end() ;)
  {
      if( !(*it).alive() ) 
      {
        Serial.print( "Delete   ");
        it = AllDelay.erase(it);
      }
      else
      {
        (*it).call();
        ++it;
      }
  }
  





  float direction = atan2(x_ctrl, -y_ctrl);
  float turn = mapf(x_turn, -1, 1, -180, 180);
  float speed = sqrt((pow(x_ctrl, 2) + pow(y_ctrl, 2))) * 255;
  if( !onMoveExecute ){
    movebase(speed, direction, turn);
  }
  

  // -------main loop for stall Telescope -------- //
  //if(isTeleStall) TeleStall();


  if( isAutoUp ) TeleAutoUp();
  if( isAutoDown ) TeleAutoDown();
  if( isTeleStall )   TeleStall();

  if( isKeep )
  {
    Grip7.run( -150 );
    Grip8.run( 150 );
  }
  else
  {
    Grip7.run( 80 );
    Grip8.run( -80 );
  }

  //Serial.println( teleEncoder.getCount() ); // check TeleEncoder

  // --------------------------------------------- //

  //Serial.println( x );
  // if( isAutoAim )
  // {
  //   while( isAutoAim )
  //   {
  //     if( Serial.available() )
  //     {
  //        //x = Serial.readString().toInt();
  //        String input = Serial.readStringUntil('\n');
  //        x = input.toInt();
  //        BFver( x );
  //        Serial.println( x );
  //     }
  //   }
  // }


}
