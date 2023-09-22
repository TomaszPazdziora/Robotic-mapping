#include "init.h"

hw_timer_t *MotorSpeedTim = NULL;

const char* ssid = ""; 
const char* password = ""; 
const char* serverName = "http://192.168.0.38:5000/data";

int state = 0;  
