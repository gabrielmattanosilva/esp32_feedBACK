#ifndef FEEDBACK_HANDLER_H
#define FEEDBACK_HANDLER_H

#include <Arduino.h>

class FeedbackHandler
{
public:
    FeedbackHandler();
    void initialize();
    void startupSequence();
    void setLED(bool state);
    void blinkLED();
    void blinkError(int count);
    void stopBlinking();
    void update();
    void triggerVibration(int count, int duration);

private:
    const int VIBRATION_PIN = 4;
    const int LED_PIN = 2;

    bool ledState = false;
    bool shouldBlink = false;
    unsigned long lastBlinkTime = 0;

    unsigned long vibrationStartTime = 0;
    unsigned long vibrationDuration = 0;
    int vibrationCount = 0;
    bool vibrationActive = false;
    unsigned long lastVibrationTime = 0;
};

#endif