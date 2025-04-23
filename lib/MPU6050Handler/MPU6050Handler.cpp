#include "MPU6050Handler.h"
#include "Logger.h"

// Implementar
/*bool MPU6050Handler::testConnection() {
    return mpu.testConnection();
}*/

// Pulbic
void MPU6050Handler::initialize()
{
    Wire.begin(PIN_MPU_SDA, PIN_MPU_SCL);
    mpu.initialize();
    optimizeMPU();
    loadCalibration();
}

void MPU6050Handler::calibrate()
{
    // Log inicial com instruções para o usuário
    Logger::logMessage(ESP_LOG_INFO, "MPU6050", "Iniciando calibracao - posicione o dispositivo em superficie plana e mantenha imovel");

    // Calibração do acelerômetro
    Logger::logMessage(ESP_LOG_DEBUG, "MPU6050", "Iniciando calibracao do acelerometro (6 iteracoes)");
    mpu.CalibrateAccel(6);
    Logger::logMessage(ESP_LOG_DEBUG, "MPU6050", "Calibracao do acelerometro concluida");

    // Calibração do giroscópio
    Logger::logMessage(ESP_LOG_DEBUG, "MPU6050", "Iniciando calibracao do giroscopio (6 iteracoes)");
    mpu.CalibrateGyro(6);
    Logger::logMessage(ESP_LOG_DEBUG, "MPU6050", "Calibracao do giroscopio concluida");

    // Calcula os offsets
    calData.accelX_offset = mpu.getXAccelOffset() / 16384.0;
    calData.accelY_offset = mpu.getYAccelOffset() / 16384.0;
    calData.accelZ_offset = (mpu.getZAccelOffset() - 16384) / 16384.0;

    calData.gyroX_offset = mpu.getXGyroOffset() / 131.0;
    calData.gyroY_offset = mpu.getYGyroOffset() / 131.0;
    calData.gyroZ_offset = mpu.getZGyroOffset() / 131.0;

    // Log dos offsets calculados
    Logger::logMessage(ESP_LOG_DEBUG, "MPU6050", "Offsets do acelerometro:");
    Logger::logMessage(ESP_LOG_DEBUG, "MPU6050", "  X: %.5f", calData.accelX_offset);
    Logger::logMessage(ESP_LOG_DEBUG, "MPU6050", "  Y: %.5f", calData.accelY_offset);
    Logger::logMessage(ESP_LOG_DEBUG, "MPU6050", "  Z: %.5f", calData.accelZ_offset);

    Logger::logMessage(ESP_LOG_DEBUG, "MPU6050", "Offsets do giroscopio:");
    Logger::logMessage(ESP_LOG_DEBUG, "MPU6050", "  X: %.5f", calData.gyroX_offset);
    Logger::logMessage(ESP_LOG_DEBUG, "MPU6050", "  Y: %.5f", calData.gyroY_offset);
    Logger::logMessage(ESP_LOG_DEBUG, "MPU6050", "  Z: %.5f", calData.gyroZ_offset);

    // Salva na EEPROM
    Logger::logMessage(ESP_LOG_DEBUG, "MPU6050", "Salvando calibracao na EEPROM");
    saveCalibration();

    // Log final
    Logger::logMessage(ESP_LOG_INFO, "MPU6050", "Calibracao concluida com sucesso!");
    Logger::logMessage(ESP_LOG_INFO, "MPU6050", "Dispositivo pronto para uso");
}

SensorData MPU6050Handler::readSensorData()
{
    SensorData data;
    int16_t ax, ay, az, gx, gy, gz;

    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    data.accelX = (ax / 16384.0) - calData.accelX_offset;
    data.accelY = (ay / 16384.0) - calData.accelY_offset;
    data.accelZ = (az / 16384.0) - calData.accelZ_offset;

    data.gyroX = (gx / 131.0) - calData.gyroX_offset;
    data.gyroY = (gy / 131.0) - calData.gyroY_offset;
    data.gyroZ = (gz / 131.0) - calData.gyroZ_offset;

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

bool MPU6050Handler::isCalibrationButtonPressed()
{
    if (digitalRead(PIN_BUTTON_CALIBRATE) == LOW)
    {
        delay(50); // Debounce - definir no .h
        Logger::logMessage(ESP_LOG_INFO, "MPU6050", "Botao de calibracao pressionado");
        return digitalRead(PIN_BUTTON_CALIBRATE) == LOW;
    }
    return false;
}

// Private
void MPU6050Handler::optimizeMPU()
{
    mpu.setDLPFMode(MPU6050_DLPF_BW_5);
    mpu.setRate(4);
    mpu.setSleepEnabled(false);
    mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
    mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_250);
}

void MPU6050Handler::saveCalibration()
{
    EEPROM.begin(sizeof(CalibrationData));
    EEPROM.put(0, calData);
    EEPROM.commit();
    EEPROM.end();
}

void MPU6050Handler::loadCalibration()
{
    EEPROM.begin(sizeof(CalibrationData));
    EEPROM.get(0, calData);
    EEPROM.end();

    if (isnan(calData.accelX_offset))
    {
        memset(&calData, 0, sizeof(CalibrationData));
    }
}