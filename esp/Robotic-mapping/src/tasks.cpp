#include "tasks.h"

void idleTask() {
    // timerAlarmEnable(MotorSpeedTim); 
    LeftMotor.stop();
    RightMotor.stop();
}

void manualControlTask() {
    // gyroscope.updateAngles();
    LeftMotor.move();
    RightMotor.move();
}

