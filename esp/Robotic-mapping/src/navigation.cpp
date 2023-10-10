#include "navigation.h"

Motor RightMotor = Motor(Pin_RightMotorForward, Pin_RightMotorBackward, Pin_RightMotorPWM);
Motor LeftMotor = Motor(Pin_LeftMotorForward, Pin_LeftMotorBackward, Pin_LeftMotorPWM);

void Motor::move() {

    if (speed >= 0) { // Set Motor forwards
        digitalWrite(Pin_Forward, HIGH);
        digitalWrite(Pin_Backward, LOW);

        if (speed >= 255) 
            analogWrite(Pin_PWM, 255);
        else
            analogWrite(Pin_PWM, speed);
    }

    else { // Set Motor left backwards
        digitalWrite(Pin_Forward, LOW);
        digitalWrite(Pin_Backward, HIGH);

        if (speed <= -255) 
            analogWrite(Pin_PWM, 255);
        else
            analogWrite(Pin_PWM, -speed);
    } 
}

void Motor::setSpeed(int s) {
    this->speed = s;
}

void Motor::stop() {
    speed = 0;
    digitalWrite(Pin_Forward, LOW);
    digitalWrite(Pin_Backward, LOW);
    analogWrite(Pin_PWM, 0);
}

void IRAM_ATTR MotorSpeedInterrupt() {
    timerAlarmDisable(MotorSpeedTim); 
    float rotationSpeed = (leftEncoderCnt / DISC_SLOTS) * 60;
    Serial.print("Left motor speed: ");        
    Serial.println(rotationSpeed);  
    leftEncoderCnt = 0;

    rotationSpeed = (rightEncoderCnt / DISC_SLOTS) * 60;
    Serial.print("Right motor speed: ");        
    Serial.println(rotationSpeed);  
    rightEncoderCnt = 0;

    timerAlarmEnable(MotorSpeedTim); 
}
