#ifndef __ODOMETRY_H__
#define __ODOMETRY_H__

#include "init.h"

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
        int getAngle();
};

extern Gyroscope gyroscope;

#define DISC_SLOTS 20
#define WHEEL_DIAMETER 60.10
extern volatile int leftEncoderCnt;
extern volatile int rightEncoderCnt;

extern void incrementLeftEncoderCnt();
extern void incrementRightEncoderCnt();

#endif