#include "Hummerbot.h"
#include "BluetoothHandle.h"
#include "ProtocolParser.h"
#include "KeyMap.h"
#include "debug.h"

#define IN1_PIN 6   // PWMB
#define IN2_PIN 10  // DIRB  ---  right
#define IN3_PIN 5   // DIRA  ---  left
#define IN4_PIN 9   // PWMA

#define IR_PIN 12
#define SERVO_PIN 13
#define UL_SING_PIN 3
#define UL_RGB_PIN 2
#define PS2X_CLK 11
#define PS2X_CMD 7
#define PS2X_CS  8
#define PS2X_DAT 4
#define IR_TRACING_PIN1 A0
#define IR_TRACING_PIN2 A1
#define IR_TRACING_PIN3 A2

#define PHOTOSENSITIVE_LEFT_PIN A2
#define PHOTOSENSITIVE_RIGHT_PIN A4
#define IR_AVOIDANCE_LEFT_PIN A3
#define IR_AVOIDANCE_RIGHT_PIN A5

ProtocolParser *mProtocol = new ProtocolParser();
Hummerbot hbot(mProtocol, IN1_PIN, IN2_PIN, IN3_PIN, IN4_PIN);
byte Ps2xStatus, Ps2xType;

void setup()
{
  Serial.begin(9600);
  hbot.init();
  hbot.SetControlMode(E_ULTRASONIC_INFRARED_AVOIDANCE);
  hbot.SetRgbUltrasonicPin(UL_SING_PIN, UL_RGB_PIN, SERVO_PIN);
  hbot.SetPhotoInfraredAvoidancePin(IR_AVOIDANCE_LEFT_PIN, IR_AVOIDANCE_RIGHT_PIN, PHOTOSENSITIVE_LEFT_PIN, PHOTOSENSITIVE_RIGHT_PIN);
  hbot.SetSpeed(0);
  hbot.mRgbUltrasonic->SetServoBaseDegree(90);
  hbot.mRgbUltrasonic->SetServoDegree(90);

}
void HandleUltrasonicInfraredAvoidance(void)
{
  uint16_t RightValue, LeftValue;
  uint16_t UlFrontDistance, UlLeftDistance, UlRightDistance;
  LeftValue = hbot.GetInfraredAvoidanceValue(0);
  RightValue = hbot.GetInfraredAvoidanceValue(1);
  UlFrontDistance =  hbot.GetUltrasonicValue(FRONT);
  Serial.println(UlFrontDistance);
  delay(20);
  DEBUG_LOG(DEBUG_LEVEL_INFO, "UlFrontDistance =%d \n", UlFrontDistance);
  if ((RightValue != IA_THRESHOLD) && (LeftValue == IA_THRESHOLD)) {
    hbot.SetSpeed(70);
    hbot.Drive(20);
  } else if ((RightValue == IA_THRESHOLD) && (LeftValue != IA_THRESHOLD)) {
    hbot.SetSpeed(70);
    hbot.Drive(160);
  } else {
    hbot.SetSpeed(45);
    hbot.GoForward();
  }
  DEBUG_LOG(DEBUG_LEVEL_INFO, "UlFrontDistance = %d \n\r", UlFrontDistance);
  if (UlFrontDistance < UL_LIMIT_MID) {
    hbot.KeepStop();
    if (UlFrontDistance <= UL_LIMIT_MIN || (RightValue == IA_THRESHOLD && LeftValue == IA_THRESHOLD)) {
      hbot.SetSpeed(60);
      hbot.GoBack();
      delay(300);
      hbot.KeepStop();
    }
    UlLeftDistance = hbot.GetUltrasonicValue(LEFT);
    UlRightDistance = hbot.GetUltrasonicValue(RIGHT);
    if (UlRightDistance >= UlLeftDistance) {
      hbot.SetSpeed(100);
      hbot.TurnRight();
      delay(400);
    }
    if (UlLeftDistance > UlRightDistance) {
      hbot.SetSpeed(100);
      hbot.TurnLeft();
      delay(400);
    }
    if (UlLeftDistance <= UL_LIMIT_MIN && UlRightDistance <= UL_LIMIT_MIN ) {
      hbot.SetSpeed(100);
      hbot.Drive(0);
      delay(800);
      hbot.KeepStop();
    }
    hbot.KeepStop();
  }
}

void loop()
{
  switch (hbot.GetControlMode()) {
    case E_ULTRASONIC_INFRARED_AVOIDANCE:
      DEBUG_LOG(DEBUG_LEVEL_INFO, "E_ULTRASONIC_INFRARED_AVOIDANCE \n");
      HandleUltrasonicInfraredAvoidance();
      break;
    default:
      break;
  }
  switch (hbot.GetStatus()) {
    case E_FORWARD:
      hbot.SetRgbColor(E_RGB_ALL, RGB_WHITE);
      break;
    case E_LEFT:
      hbot.SetRgbColor(E_RGB_LEFT, RGB_WHITE);
      break;
    case E_RIGHT:
      hbot.SetRgbColor(E_RGB_RIGHT, RGB_WHITE);
      break;
    case E_BACK:
      hbot.SetRgbColor(E_RGB_ALL, RGB_RED);
      break;
    case E_STOP:
      hbot.SetRgbColor(E_RGB_ALL, RGB_OFF);
      break;
    case E_SPEED_UP:
      hbot.SetRgbColor(E_RGB_ALL, hbot.GetSpeed() * 2.5);
      break;
    case E_SPEED_DOWN:
      hbot.SetRgbColor(E_RGB_ALL, hbot.GetSpeed() * 2.5);
      break;
    default:
      break;
  }
}
