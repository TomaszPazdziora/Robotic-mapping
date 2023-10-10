#include "tasks.h"


void idleTask() {
    while(state == idle) {
        readDataFromServer();
        LeftMotor.stop();
        RightMotor.stop();
    }
}

void manualControlTask() {

    while(state == manual) {
        readDataFromServer();
        LeftMotor.move();
        RightMotor.move();        
    }
    // timerAlarmEnable(MotorSpeedTim); 
}

#define NOTHING    0
#define FORWARDS   1
#define TURN_LEFT  2
#define TURN_RIGHT 3

void traceTask() {
    while(state == trace) {
        readDataFromServer();
        String trace = httpGETRequest(serverTraceAddress);
        Serial.println(trace);

        int cmd = 0;
        String value = "";
        for(int i=0; i<trace.length(); i++) {
            readDataFromServer();
            if (trace[i] == 'f') cmd = FORWARDS;
            else if (trace[i] == 'l') cmd = TURN_LEFT;
            else if (trace[i] == 'r') cmd = TURN_RIGHT;
            else if (trace[i] >= 48 && trace[i] <= 57) {
                value += trace[i];
            }
            else if (trace[i] == ',') {
                int v = value.toInt();
                // Serial.print(cmd);
                // Serial.print(": ");
                // Serial.println(value);

                switch(cmd) {
                    case NOTHING:
                        Serial.println("nothing");
                        break;
                    case FORWARDS:
                        Serial.println("frowards");
                        moveXCM(v);
                        break;
                    case TURN_LEFT:
                        Serial.println("turning left"); 
                        turnLeft(v);
                        break;
                    case TURN_RIGHT:
                        Serial.println("turning right");
                        turnRight(v);
                        break;
                    default:
                        Serial.println("turning right");
                        break;
                }
                cmd = 0;
                value = "";
                delay(100);
            }
        }
        LeftMotor.stop();
        RightMotor.stop();
        delay(1000);     
    }
}

void sandboxTask() {
    Serial.println(gyroscope.getAngle());
    // timerAlarmDisable(MotorSpeedTim);
    delay(300);
}


// ###################################################
// ##           Motor control functions             ##
// ###################################################

void moveXSteps(int x) {
    Serial.print("Steps: ");
    Serial.println(x);
    leftEncoderCnt = 0;
    rightEncoderCnt = 0;
    // timerAlarmEnable(MotorSpeedTim); 

    LeftMotor.move();
    RightMotor.move();

    while(leftEncoderCnt <= x && rightEncoderCnt <= x) {readDataFromServer();}

    LeftMotor.stop();
    RightMotor.stop();
}

void moveXCM(int x) {
    float steps = x / stepInCm;
    // Serial.println(round(steps));
    moveXSteps(round(steps));
}

void turnLeft(int degrees) {
    int goalAngle = gyroscope.getAngle() - degrees;
    if (goalAngle <= -180) {
        goalAngle = 180 - goalAngle;
        RightMotor.move();
        while(gyroscope.getAngle() < 0) {readDataFromServer();}
    }
    RightMotor.move();
    while(gyroscope.getAngle() >= goalAngle) {readDataFromServer();}
    RightMotor.stop();
}

void turnRight(int degrees) {
    int goalAngle = gyroscope.getAngle() + degrees;
    if (goalAngle >= 180) {
        goalAngle = -180 + (goalAngle - 180);
        LeftMotor.move();
        while(gyroscope.getAngle() > 0) {readDataFromServer();}
    }
    LeftMotor.move();
    while(gyroscope.getAngle() <= goalAngle) {readDataFromServer();}
    LeftMotor.stop();
}
