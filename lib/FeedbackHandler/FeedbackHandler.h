#ifndef FEEDBACK_HANDLER_H
#define FEEDBACK_HANDLER_H

#include <Arduino.h>

class FeedbackHandler
{
public:
    FeedbackHandler();
    void initialize();
    void update(); // Novo método para atualização não-bloqueante
    void triggerVibration(int count, int duration);
    void setLED(bool state);
    void blinkLED();
    void startupSequence();
    void blinkError(int count);

private:
    const int VIBRATION_PIN = 4;
    const int LED_PIN = 2;

    // Variáveis para controle não-bloqueante
    unsigned long vibrationStartTime = 0;
    unsigned long vibrationDuration = 0;
    int vibrationCount = 0;
    bool vibrationActive = false;
    unsigned long lastVibrationTime = 0;
    unsigned long lastBlinkTime = 0;
    bool ledState = false;
};

#endif