#include "init.h"

hw_timer_t *MotorSpeedTim = NULL;

const char* ssid = "* UPC6948437-2.4"; 
const char* password = "vESck6wbmddj"; 
const char* serverDataAddress =             "http://192.168.0.38:5000/data";
const char* serverTraceAddress =            "http://192.168.0.38:5000/trace_cmd_data";
const char* serverLidarReadyAddress =       "http://192.168.0.38:5000/ready_to_scan";
const char* serverReadyForTraceAddress =    "http://192.168.0.38:5000/ready_for_trace";
const char* serverCurrentPosition =         "http://192.168.0.38:5000/current_position";
const char* serverLidarDataSent =           "http://192.168.0.38:5000/scan_sent";

int state = 0;  
