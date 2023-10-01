#ifndef __NAVIGATION_H__
#define __NAVIGATION_H__

#include "odometry.h"

class Motor {
    private:
        int Pin_Forward;
        int Pin_Backward;
        int Pin_PWM;
        int speed;

    public: 
    Motor(int forw, int back, int pwm) {
        this->Pin_Forward = forw;
        this->Pin_Backward = back;
        this->Pin_PWM = pwm;
        speed = 0;
    }

    void move();
    void moveXSteps(int x);
    void moveXCM(int x);
    void setSpeed(int s);
    void stop();
};

extern Motor RightMotor;
extern Motor LeftMotor;
void IRAM_ATTR MotorSpeedInterrupt();

#endif