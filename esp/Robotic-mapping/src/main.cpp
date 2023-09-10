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
  
  attachInterrupt(digitalPinToInterrupt (Pin_LeftEncoder), LeftEncoder.incrementPulseCnt, RISING);
  attachInterrupt(digitalPinToInterrupt (Pin_RightEncoder), RightEncoder.incrementPulseCnt, RISING);  
  delay(500);
  
  gyroscope.init();
  // WiFi.begin(ssid, password);
  // Serial.println("Connecting to Wifi");

  // while(WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }
  // Serial.print("\nConnected\n");
  

}

void loop() {
  // delay(100);
  // readDataFromServer();

  delay(500);
  gyroscope.updateAngles();

  // delay(1000);
  // switch(state) {
  //   case idle:
  //     Serial.println("idle");
  //     idleTask();
  //     break;
  //   case manual:
  //     Serial.println("manual");
  //     manualControlTask();
  //     break;
  //   default:
  //     Serial.println("diff");
  //     break;
  // }
}

