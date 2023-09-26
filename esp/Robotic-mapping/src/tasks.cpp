#include "tasks.h"

void idleTask() {
    while(state == idle) {
        readDataFromServer();
        timerAlarmDisable(MotorSpeedTim);
        LeftMotor.stop();
        RightMotor.stop();
    }
}

void manualControlTask() {

    while(state == manual) {
        readDataFromServer();
        // int angle = gyroscope.getAngle();
        LeftMotor.move();
        RightMotor.move();        
    }
    
    // timerAlarmEnable(MotorSpeedTim); 
}

