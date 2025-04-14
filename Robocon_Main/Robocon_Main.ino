#include <Arduino.h>
#include <ESP32Encoder.h>
#include <Wire.h>
#include <PID_v2.h>
#include <math.h>
#include <Bluepad32.h>
#include "Mapf.h"

#define Left_I2C_ADDRESS 0x50
#define Right_I2C_ADDRESS 0x57
#define Front_I2C_ADDRESS 0x85
#define Back_I2C_ADDRESS 0x86

#define MOTOR_SDA 21
#define MOTOR_SCL 22
#define MAX_RPM 500

/*---------Lecate--------------*/
#define Lecate_I2C_ADDRESS 0x54
#define LeftLecatePin 0x85
#define RightLecatePin 0x86

/*---------Test Lecate Button---------*/
bool Xpressed = false;

/*---------Gripper---------*/
#define Gripper_I2C_ADDRESS 0x56
#define LeftGripperPin 0x86
#define RightGripperPin 0x85

/*----------/Telescopic------------*/
// int IN1A = 17;
// int IN2A = 16;
#define FEEDER_I2C_ADDRESS 0x51
#define FeederPin 0x85
// #define RightTelescopicPin 0x86
#define TelePinA 18
#define TelePinB 19
#define TelePWM 17

bool isTeleStall = false;
bool isAutoUp = false;
bool isAutoDown = false;

bool lecateDelay = false; // use for is Auto Down
long peakStall;

//saddwadaw
bool isAutoAim = false;


//Telescope encoder
ESP32Encoder teleEncoder ;

//encoder Pin
#define teleEncoderPinA 14
#define teleEncoderPinB 13



/* ---------------- CONTROLLER INIT ---------------- */

ControllerPtr myControllers[BP32_MAX_GAMEPADS];
float x_ctrl = 0;
float y_ctrl = 0;
float x_turn = 0;
float y_turn = 0;

/* ---------------- ENOCODERS INIT ---------------- */
// Define pin numbers for encoder A and B channels
const int encoder1PinA = 32;
const int encoder1PinB = 33;

const int encoder2PinA = 25;
const int encoder2PinB = 26;

const int encoder3PinA = 14;
const int encoder3PinB = 27;

const int encoder4PinA = 12;
const int encoder4PinB = 13;

// Create an encoder object
ESP32Encoder encoder1;
ESP32Encoder encoder2;
ESP32Encoder encoder3;
ESP32Encoder encoder4;



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

double Kp = 0.7, Ki = 5, Kd = 0;
PID_v2 PID_encoder1(Kp, Ki, Kd, PID::Direct);
PID_v2 PID_encoder2(Kp, Ki, Kd, PID::Direct);
PID_v2 PID_encoder3(Kp, Ki, Kd, PID::Direct);
PID_v2 PID_encoder4(Kp, Ki, Kd, PID::Direct);

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
  resetEncoders();

	Serial.setTimeout(1);

  pinMode(TelePinA, OUTPUT);
  pinMode(TelePinB, OUTPUT);
  pinMode(TelePWM, OUTPUT);

  //Serial.setTimeout(1);

  // /* ---------------- TEST ---------------- */
  // Serial.println("Starting...");
  // delay(6000);
  // Serial.println("Started!!!");

  
}

void loop() {
  //while (!Serial.available());

  //Serial.println( "Loop available" ) // for check loop

  calculateRPMs();
  bool dataUpdated = BP32.update();
  if (dataUpdated) {
    //Serial.print("data updated");
    processControllers();
  }

  //Serial.println( teleEncoder.getCount() );
  // TeleUp(200,500);





  float direction = atan2(x_ctrl, -y_ctrl);
  float turn = mapf(x_turn, -1, 1, -180, 180);
  float speed = sqrt((pow(x_ctrl, 2) + pow(y_ctrl, 2))) * 255;
  movebase(speed, direction, turn);

  // -------main loop for stall Telescope -------- //
  //if(isTeleStall) TeleStall();


  if( isAutoUp ) TeleAutoUp();
  if( isAutoDown ) TeleAutoDown();
  if( isTeleStall )   TeleStall();

  //Serial.println( teleEncoder.getCount() ); // check TeleEncoder

  // --------------------------------------------- //

  //Serial.println( x );
  if( isAutoAim )
  {
    while( isAutoAim )
    {
      if( Serial.available() )
      {
        x = Serial.readString().toInt();
        BFver( x );
        Serial.println( x );

      }
    }
  }


  // motor(6,-255); // motor 5 + backward - forward
  // motor(5,-255);
  // delay(200);
  // motor(5,250);
  // delay(4000);

  // vTaskDelay(1);
  // analogWrite(PWM, 255);
  // digitalWrite(INA, HIGH);
  // digitalWrite(INB, LOW);

<<<<<<< HEAD
=======


  // motor(9, -185);
  // motor(10, -185);
>>>>>>> parent of e60a285 (Before restructure code)
}