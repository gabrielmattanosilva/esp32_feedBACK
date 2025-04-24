#ifndef MPU6050_HANDLER_H
#define MPU6050_HANDLER_H

#include <Wire.h>
#include <MPU6050.h>
#include "Pins.h"

enum PostureState
{
    POSTURE_GOOD,
    POSTURE_WARNING,
    POSTURE_BAD
};

struct SensorData
{
    float accelX, accelY, accelZ;
    float gyroX, gyroY, gyroZ;
};

class MPU6050Handler
{
public:
    // bool testConnection(); - implementar
    void initialize();
    SensorData readSensorData();
    PostureState evaluatePosture(float pitch, float roll);
    unsigned long getBadPostureTimeThreshold() const { return 5000; }

private:
    MPU6050 mpu;
    void optimizeMPU();
};
#endif