#include "init.h"

hw_timer_t *MotorSpeedTim = NULL;

const char* ssid = ""; 
const char* password = ""; 
const char* serverDataAddress = "http://192.168.0.38:5000/data";
const char* serverTraceAddress = "http://192.168.0.38:5000/trace_cmd_data";

int state = 0;  
