#include "tasks.h"

void setup() {
  Wire.begin();
  Wire.setClock(400000);
  Serial.begin(115200);

  pinMode(Pin_LeftMotorBackward,  OUTPUT);
  pinMode(Pin_LeftMotorForward,   OUTPUT);
  pinMode(Pin_LeftMotorPWM,       OUTPUT);
  pinMode(Pin_RightMotorBackward, OUTPUT);
  pinMode(Pin_RightMotorForward,  OUTPUT);
  pinMode(Pin_RightMotorPWM,      OUTPUT);
  gyroscope.init();
  
  // MotorSpeedTim = timerBegin(0, 80, true);
  // timerAttachInterrupt(MotorSpeedTim, &MotorSpeedInterrupt, true);
  // timerAlarmWrite(MotorSpeedTim, 1000000, true);

  attachInterrupt(digitalPinToInterrupt (Pin_LeftEncoder), incrementLeftEncoderCnt, RISING);
  attachInterrupt(digitalPinToInterrupt (Pin_RightEncoder), incrementRightEncoderCnt, RISING);  
  delay(500);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting to Wifi");

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nConnected\n");
}

void loop() {
  switch(state) {
    case idle:
      Serial.println("idle");
      idleTask();
      break;
    case manual:
      Serial.println("manual");
      manualControlTask();
      break;
    case trace:
      Serial.println("trace");
      traceTask();
    default:
      Serial.println("diff");
      break;
  }
}

