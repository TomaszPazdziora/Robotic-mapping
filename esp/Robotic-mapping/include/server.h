#ifndef __SERVER_H__
#define __SERVER_H__

#include "init.h"

extern String sensorReadings;
extern int sensorReadingsArr[3];

String httpGETRequest(const char* serverName);
void readDataFromServer();

#endif
