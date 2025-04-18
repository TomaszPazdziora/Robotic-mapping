#include "tasks.h"


void idleTask() {
    while(state == idle) {
        readDataFromServer();
        LeftMotor.stop();
        RightMotor.stop();
        delay(100);
    }
}

void manualControlTask() {

    while(state == manual) {
        readDataFromServer();
        LeftMotor.move();
        RightMotor.move();
        delay(50);        
    }
    // timerAlarmEnable(MotorSpeedTim); 
}

#define NOTHING    0
#define FORWARDS   1
#define TURN_LEFT  2
#define TURN_RIGHT 3
#define SCAN       4

const float lidarToWheelRadius = 40;

void traceTask() {
    Position CurrentPosition, LastPosition;
    CurrentPosition.x = 0;
    CurrentPosition.y = 0;
    CurrentPosition.angle = 0.0;
    int beforeShift = 0;

    while(state == trace) {
        readDataFromServer();
        String isRobotReady = httpGETRequest(serverReadyForTraceAddress);

        if (isRobotReady == "ready") {
            String trace = httpGETRequest(serverTraceAddress);
            Serial.println(trace);
            int cmd = 0;

            String value = "";
            for(int i=0; i<trace.length(); i++) {
                readDataFromServer();
                if (trace[i] == 'f') cmd = FORWARDS;
                else if (trace[i] == 'l') cmd = TURN_LEFT;
                else if (trace[i] == 'r') cmd = TURN_RIGHT;
                else if (trace[i] == 's') cmd = SCAN;
                else if (trace[i] >= 48 && trace[i] <= 57) {
                    value += trace[i];
                }
                else if (trace[i] == ',') {
                    float v = value.toFloat();
                    // Serial.print(cmd);
                    // Serial.print(": ");
                    // Serial.println(value);

                    switch(cmd) {
                        case NOTHING:
                            Serial.println("nothing");
                            break;
                        case FORWARDS:
                            Serial.println("frowards");
                            // moveXMiliSec(v);
                            CurrentPosition.angle = gyroscope.getAngle();
                            moveXCM(v);
                            v = v * 0.6;
                            CurrentPosition.angle = gyroscope.getAngle();
                            CurrentPosition.x -= v*10*sin(CurrentPosition.angle*(PI/180));
                            CurrentPosition.y -= v*10*cos(CurrentPosition.angle*(PI/180));
                            sendCurrPosToServer(CurrentPosition);
                            break;
                        case TURN_LEFT:
                            Serial.println("turning left"); 
                            // turnLeft(v);
                            // CurrentPosition.angle = gyroscope.getAngle();
                            // sendCurrPosToServer(CurrentPosition);
                            beforeShift = gyroscope.getAngle();
                            CurrentPosition.x = lidarToWheelRadius * sin(beforeShift*(PI/180)) + CurrentPosition.x;
                            CurrentPosition.y = lidarToWheelRadius * cos(beforeShift*(PI/180)) + CurrentPosition.y;
                            turnLeft(v);
                            CurrentPosition.angle = gyroscope.getAngle();
                            CurrentPosition.x = lidarToWheelRadius * sin(CurrentPosition.angle*(PI/180)) + CurrentPosition.x;
                            CurrentPosition.y = lidarToWheelRadius * cos(CurrentPosition.angle*(PI/180)) + CurrentPosition.y;
                            sendCurrPosToServer(CurrentPosition);
                            break;
                        case TURN_RIGHT:
                            Serial.println("turning right");
                            // turnRight(v);
                            // CurrentPosition.angle = gyroscope.getAngle();
                            // sendCurrPosToServer(CurrentPosition);
                            beforeShift = gyroscope.getAngle();
                            CurrentPosition.x = lidarToWheelRadius * sin(beforeShift*(PI/180)) + CurrentPosition.x;
                            CurrentPosition.y = lidarToWheelRadius * cos(beforeShift*(PI/180)) + CurrentPosition.y;
                            turnRight(v);
                            CurrentPosition.angle = gyroscope.getAngle();
                            CurrentPosition.x = lidarToWheelRadius * sin(CurrentPosition.angle*(PI/180)) + CurrentPosition.x;
                            CurrentPosition.y = lidarToWheelRadius * cos(CurrentPosition.angle*(PI/180)) + CurrentPosition.y;
                            sendCurrPosToServer(CurrentPosition);
                            break;
                        case SCAN:
                            sendCurrPosToServer(CurrentPosition);
                            delay(500);
                            readyForScan();
                            while (isLidarDataSent() == false){delay(100);};
                            break;
                        default:
                            Serial.println("default");
                            break;
                    }
                    cmd = 0;
                    value = "";
                    delay(100);
                }
            }
        }
        else {
            Serial.println("NOT READY");
        }
        delay(1000);     
    }
}


void scanningTask() {
    Position CurrentPosition;
    CurrentPosition.x = 0;
    CurrentPosition.y = 0;
    CurrentPosition.angle = 0.0;
    // to do
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
    attachInterrupt(digitalPinToInterrupt (Pin_LeftEncoder), incrementLeftEncoderCnt, RISING);
    attachInterrupt(digitalPinToInterrupt (Pin_RightEncoder), incrementRightEncoderCnt, RISING); 

    LeftMotor.move();
    RightMotor.move();

    while(leftEncoderCnt <= x && rightEncoderCnt <= x) {
        // readDataFromServer();
        // if (LeftMotor.speed == 0 ||
        // RightMotor.speed == 0) break;
        // delay(10);
    }

    detachInterrupt(digitalPinToInterrupt (Pin_LeftEncoder));
    detachInterrupt(digitalPinToInterrupt (Pin_RightEncoder)); 

    LeftMotor.stop();
    RightMotor.stop();
}

void moveXCM(int x) {
    float steps = x / stepInCm;
    Serial.println(round(steps));
    moveXSteps(round(steps));
}

void moveXMiliSec(int x) {
    LeftMotor.move();
    RightMotor.move();
    delay(x);
    LeftMotor.stop();
    RightMotor.stop();
}

void turnLeft(int degrees) {
    int goalAngle = gyroscope.getAngle() - degrees;
    if (goalAngle <= -180) {
        goalAngle = 180 - goalAngle;
        RightMotor.move();
        while(gyroscope.getAngle() < 0) {
            readDataFromServer();
            if (RightMotor.speed == 0) break;
            // delay(10);
        }
    }
    RightMotor.move();
    while(gyroscope.getAngle() >= goalAngle) {
        readDataFromServer();
        if (RightMotor.speed == 0) break;
        // delay(10);s
    }
    RightMotor.stop();
}

void turnRight(int degrees) {
    int goalAngle = gyroscope.getAngle() + degrees;
    if (goalAngle >= 180) {
        goalAngle = -180 + (goalAngle - 180);
        LeftMotor.move();
        while(gyroscope.getAngle() > 0) {
            readDataFromServer();
            if (LeftMotor.speed == 0) break;
            // delay(10);
        }
    }
    LeftMotor.move();
    while(gyroscope.getAngle() <= goalAngle) {
        readDataFromServer();
        if (LeftMotor.speed == 0) break;
        // delay(10);
    }
    LeftMotor.stop();
}
