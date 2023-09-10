#ifndef __INIT_H__
#define __INIT_H__

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

// DEFINE ALL PERIPHERALS

#define Pin_LeftMotorForward 14 // zmien na 26
#define Pin_LeftMotorBackward 27
#define Pin_LeftMotorPWM 26 // zmien na 14

#define Pin_RightMotorForward 25
#define Pin_RightMotorBackward 33
#define Pin_RightMotorPWM 32

#define Pin_Trigger 15
#define Pin_Echo 21

#define Pin_RightEncoder 4
#define Pin_LeftEncoder 0


// WIFI DATA 

extern const char* ssid; 
extern const char* password;
extern const char* serverName;

// ENUMS AND STRUCTS

enum States { idle, manual, trace, scanning};


// GLOBAL VARIABLES

extern int state;

#endif
