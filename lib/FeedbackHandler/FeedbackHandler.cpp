#include "FeedbackHandler.h"

FeedbackHandler::FeedbackHandler() {}

void FeedbackHandler::initialize()
{
    pinMode(VIBRATION_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    startupSequence();
}

void FeedbackHandler::startupSequence()
{
    for (int i = 0; i < 3; i++)
    {
        setLED(true);
        digitalWrite(VIBRATION_PIN, HIGH);
        delay(100);
        setLED(false);
        digitalWrite(VIBRATION_PIN, LOW);
        delay(100);
    }
}

void FeedbackHandler::setLED(bool state)
{
    ledState = state;
    digitalWrite(LED_PIN, ledState);
}

void FeedbackHandler::blinkLED()
{
    shouldBlink = true;
}

void FeedbackHandler::blinkError(int count) // Ainda não foi implementada, usar para indicar falha na I2C, BLE, etc...
{
    for (int i = 0; i < count; i++)
    {   // 200ms entre as piscadas - definir no .h
        setLED(true);
        delay(200);
        setLED(false);
        delay(200);
    }
}

void FeedbackHandler::stopBlinking()
{
    shouldBlink = false;
    digitalWrite(LED_PIN, LOW);
}

void FeedbackHandler::triggerVibration(int count, int duration)
{
    vibrationCount = count;
    vibrationDuration = duration;
    vibrationStartTime = millis();
    vibrationActive = true;
    lastVibrationTime = millis();
}

void FeedbackHandler::update()
{
    if (shouldBlink && millis() - lastBlinkTime >= 500) // 500ms entre as piscadas - definir no .h
    {
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState);
        lastBlinkTime = millis();
    }

    if (vibrationActive)
    {
        if (millis() - vibrationStartTime >= vibrationDuration)
        {
            digitalWrite(VIBRATION_PIN, LOW);
            vibrationCount--;

            if (vibrationCount > 0)
            {
                vibrationStartTime = millis() + 200; // 200ms entre vibrações - definir no .h
            }
            else
            {
                vibrationActive = false;
            }
        }
        else if (millis() >= vibrationStartTime)
        {
            digitalWrite(VIBRATION_PIN, HIGH);
        }
    }
}