#include "navigation.h"

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

void Motor::moveXSteps() {

}

void Motor::moveXCM() {

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