#include "odometry.h"

Gyroscope gyroscope;

void Gyroscope::init() {
    mpu.initialize();
    devStatus = mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788);

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        mpu.CalibrateAccel(6);
        mpu.CalibrateGyro(6);
        mpu.PrintActiveOffsets();
        mpu.setDMPEnabled(true);
        dmpReady = true;
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }
}

int Gyroscope::getAngle() {
    if (!dmpReady) return 0;
    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) { // Get the Latest packet 
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
            Serial.print("ypr\t");
            Serial.println(ypr[0] * 180/M_PI);
            // Serial.print("\t");
            // Serial.print(ypr[1] * 180/M_PI);
            // Serial.print("\t");
            // Serial.println(ypr[2] * 180/M_PI);
    }
    return ypr[0] * 180/M_PI;
}

volatile int leftEncoderCnt = 0;
volatile int rightEncoderCnt = 0;
float stepInCm = WHEEL_DIAMETER * 3.14 / DISC_SLOTS;
// float stepInCm = 6.10 * 3.14 / 20;

void incrementLeftEncoderCnt() {
    leftEncoderCnt++;
    Serial.print("left: ");
    Serial.println(leftEncoderCnt);
}

void incrementRightEncoderCnt() {
    rightEncoderCnt++;
    Serial.print("right: ");
    Serial.println(rightEncoderCnt);
}
