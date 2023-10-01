#include "tasks.h"

void idleTask() {
    while(state == idle) {
        readDataFromServer();
        // timerAlarmDisable(MotorSpeedTim);
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

#define NOTHING    0
#define FORWARDS   1
#define TURN_LEFT  2
#define TURN_RIGHT 3

void traceTask() {
    while(state == trace) {
        readDataFromServer();
        String trace = httpGETRequest(serverTraceAddress);
        Serial.println(trace);
        // int angle = gyroscope.getAngle();  
        // LeftMotor.moveXSteps(10);
        
        int cmd = 0;
        String value;
        for(int i=0; i<trace.length(); i++) {
            if (trace[i] == 'f') cmd = FORWARDS;
            else if (trace[i] == 'l') cmd = TURN_LEFT;
            else if (trace[i] == 'r') cmd = TURN_RIGHT;
            else if (trace[i] >= 48 && trace[i] <= 57) {
                value += trace[i];
            }
            else if (trace[i] == ',') {
                int v = value.toInt();
                Serial.print(cmd);
                Serial.print(": ");
                Serial.println(value);
                cmd = 0;
                value = "";
            }
        }

        delay(4000);     
    }
}

