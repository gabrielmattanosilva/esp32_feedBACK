#include "PostureMonitor.h"
#include <MadgwickAHRS.h>

Madgwick filter;

void PostureMonitor::initialize()
{
    Logger::init();
    mpuHandler.initialize();
    feedbackHandler.initialize();
    filter.begin(20);
}

void PostureMonitor::update()
{
    // Atualiza feedback (não-bloqueante)
    feedbackHandler.update();

    // Verifica botão de calibração (não-bloqueante)
    if (mpuHandler.isCalibrationButtonPressed())
    {
        mpuHandler.calibrate();
    }

    // Lê dados do sensor
    SensorData data = mpuHandler.readSensorData();

    // Atualiza filtro
    filter.updateIMU(data.gyroX, data.gyroY, data.gyroZ,
                     data.accelX, data.accelY, data.accelZ);

    // Obtém ângulos
    pitch = filter.getPitch();
    roll = filter.getRoll();

    // Avalia postura
    PostureState newPosture = mpuHandler.evaluatePosture(pitch, roll);
    bool stateChanged = (newPosture != currentPosture);

    if (stateChanged)
    {
        if (newPosture > currentPosture)
        {
            postureTimer = millis();
        }
        currentPosture = newPosture;
    }

    handlePostureState(currentPosture, stateChanged);

    // Log dos dados
    Logger::logPostureData(
        pitch,
        roll,
        stateChanged,
        getActionMessage(currentPosture));
}

void PostureMonitor::handlePostureState(PostureState state, bool stateChanged)
{
    switch (state)
    {
    case POSTURE_GOOD:
        feedbackHandler.setLED(false);
        break;

    case POSTURE_WARNING:
        feedbackHandler.blinkLED();
        if (millis() - postureTimer > mpuHandler.getBadPostureTimeThreshold())
        {
            feedbackHandler.triggerVibration(1, 200);
        }
        break;

    case POSTURE_BAD:
        feedbackHandler.setLED(true);
        if (stateChanged || (lastFeedbackTime == 0 || millis() - lastFeedbackTime > feedbackCooldown))
        {
            feedbackHandler.triggerVibration(3, 500);
            lastFeedbackTime = millis();
        }
        break;
    }
}

const char *PostureMonitor::getActionMessage(PostureState state)
{
    static char message[50];

    switch (state)
    {
    case POSTURE_WARNING:
        return "Vibracao breve";

    case POSTURE_BAD:
        if (lastFeedbackTime > 0 && (millis() - lastFeedbackTime) < feedbackCooldown)
        {
            unsigned long remaining = (feedbackCooldown - (millis() - lastFeedbackTime)) / 1000;
            snprintf(message, sizeof(message), "Vibracao prolongada | Proxima verificacao em %lus", remaining);
            return message;
        }
        return "Vibracao prolongada";

    default:
        return "Nenhuma acao";
    }
}