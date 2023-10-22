#include "init.h"

hw_timer_t *MotorSpeedTim = NULL;

const char* ssid = "Galaxy A13ECB1"; 
const char* password = "jdlq6511"; 
const char* serverDataAddress =             "http://192.168.17.144:5000/data";
const char* serverTraceAddress =            "http://192.168.17.144:5000/trace_cmd_data";
const char* serverLidarReadyAddress =       "http://192.168.17.144:5000/ready_to_scan";
const char* serverReadyForTraceAddress =    "http://192.168.17.144:5000/ready_for_trace";

int state = 0;  
