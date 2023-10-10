#ifndef __TASKS_H__
#define __TASKS_H__

#include "server.h"

void moveXSteps(int x);
void moveXCM(int x);
void turnLeft(int degrees);
void turnRight(int degrees);

void manualControlTask();
void idleTask();
void traceTask();
void sandboxTask();

#endif