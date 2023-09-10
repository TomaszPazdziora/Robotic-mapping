#ifndef __NAVIGATION_H__
#define __NAVIGATION_H__

#include "odometry.h"

Motor RightMotor = Motor(Pin_RightMotorForward, Pin_RightMotorBackward, Pin_RightMotorPWM);
Motor LeftMotor = Motor(Pin_LeftMotorForward, Pin_LeftMotorBackward, Pin_LeftMotorPWM);

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
    void moveXSteps();
    void moveXCM();
    void setSpeed(int s);
    void stop();
};

#endif