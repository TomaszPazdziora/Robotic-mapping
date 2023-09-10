#include "tasks.h"

void idleTask() {
    stopMotor(LeftMotor);
    stopMotor(RightMotor);
}

void manualControlTask() {
    moveMotor(LeftMotor);
    moveMotor(RightMotor);
}

