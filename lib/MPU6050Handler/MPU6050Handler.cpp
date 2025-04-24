#include "MPU6050Handler.h"
#include "Logger.h"

void MPU6050Handler::initialize()
{
    Wire.begin(PIN_MPU_SDA, PIN_MPU_SCL, 400000);
    mpu.initialize();
    optimizeMPU();
}

SensorData MPU6050Handler::readSensorData()
{
    SensorData data;
    int16_t ax, ay, az, gx, gy, gz;

    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    data.accelX = ax / 16384.0;
    data.accelY = ay / 16384.0;
    data.accelZ = az / 16384.0;

    data.gyroX = gx / 131.0;
    data.gyroY = gy / 131.0;
    data.gyroZ = gz / 131.0;

    return data;
}

PostureState MPU6050Handler::evaluatePosture(float pitch, float roll)
{
    float warningThreshold = 15.0;
    float badThreshold = 25.0;
    float maxAngle = max(abs(pitch), abs(roll));

    if (maxAngle > badThreshold)
        return POSTURE_BAD;
    if (maxAngle > warningThreshold)
        return POSTURE_WARNING;
    return POSTURE_GOOD;
}

void MPU6050Handler::optimizeMPU()
{
    mpu.setDLPFMode(MPU6050_DLPF_BW_5);
    mpu.setRate(4);
    mpu.setSleepEnabled(false);
    mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
    mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_250);
}