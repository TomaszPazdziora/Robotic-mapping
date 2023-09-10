#ifndef __ODOMETRY_H__
#define __ODOMETRY_H__

#include "init.h"

Gyroscope gyroscope;
Encoder LeftEncoder, RightEncoder;

class Gyroscope {
    private:
        bool dmpReady = false;
        uint8_t devStatus;     
        uint8_t fifoBuffer[64]; 
        Quaternion q;           // [w, x, y, z]         quaternion container
        VectorFloat gravity;    // [x, y, z]            gravity vector
        float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
        MPU6050 mpu;
    
    public:
        void init();
        void updateAngles();
};

class Encoder {
    private:
        static const int discSlots = 20;
        static const float wheelDiameter = 66.10f;
        unsigned int pulseCounter = 0;

    public:
        void incrementPulseCnt();
        void resetPulseCnt();
};

#endif