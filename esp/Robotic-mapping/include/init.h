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

#define Pin_LeftMotorForward 17 
#define Pin_LeftMotorBackward 16
#define Pin_LeftMotorPWM 4 

#define Pin_RightMotorForward 5
#define Pin_RightMotorBackward 18
#define Pin_RightMotorPWM 19

#define Pin_Trigger 15
#define Pin_Echo 21

#define Pin_RightEncoder 13
#define Pin_LeftEncoder 36

extern hw_timer_t *MotorSpeedTim;

// WIFI DATA 

extern const char* ssid; 
extern const char* password;
extern const char* serverDataAddress;
extern const char* serverTraceAddress;

// ENUMS AND STRUCTS

enum States { idle, manual, trace, scanning};


// GLOBAL VARIABLES

extern int state;

#endif
