#ifndef MPU6050_HANDLER_H
#define MPU6050_HANDLER_H

#include <Wire.h>
#include <MPU6050.h>
#include <EEPROM.h>

enum PostureState
{
    POSTURE_GOOD,
    POSTURE_WARNING,
    POSTURE_BAD
};

struct CalibrationData
{
    float accelX_offset;
    float accelY_offset;
    float accelZ_offset;
    float gyroX_offset;
    float gyroY_offset;
    float gyroZ_offset;
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
    void calibrate();
    SensorData readSensorData();
    PostureState evaluatePosture(float pitch, float roll);
    bool isCalibrationButtonPressed();
    unsigned long getBadPostureTimeThreshold() const { return 5000; } // Verificar

private:
    MPU6050 mpu;
    CalibrationData calData;

    void optimizeMPU();
    void loadCalibration();
    void saveCalibration();
};

#endif