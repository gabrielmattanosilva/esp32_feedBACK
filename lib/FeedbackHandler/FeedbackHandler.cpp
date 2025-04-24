#include "FeedbackHandler.h"

FeedbackHandler::FeedbackHandler() {}

void FeedbackHandler::initialize()
{
    pinMode(PIN_VIBRATION, OUTPUT);
    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, LOW);
    startupSequence();
}

void FeedbackHandler::startupSequence()
{
    for (int i = 0; i < 3; i++)
    {
        setLED(true);
        digitalWrite(PIN_VIBRATION, HIGH);
        delay(100);
        setLED(false);
        digitalWrite(PIN_VIBRATION, LOW);
        delay(100);
    }
}

void FeedbackHandler::setLED(bool state)
{
    ledState = state;
    digitalWrite(PIN_LED, ledState ? HIGH : LOW);
}

void FeedbackHandler::blinkLED()
{
    shouldBlink = true;
}

void FeedbackHandler::blinkError(int count) // Ainda não foi implementada, usar para indicar falha na I2C, BLE, etc...
{
    for (int i = 0; i < count; i++)
    {
        digitalWrite(PIN_LED, HIGH);
        delay(200);
        digitalWrite(PIN_LED, LOW);
        delay(200);
    }
}

void FeedbackHandler::stopBlinking()
{
    shouldBlink = false;
    digitalWrite(PIN_LED, LOW);
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
        digitalWrite(PIN_LED, ledState ? HIGH : LOW);
        lastBlinkTime = millis();
    }

    if (vibrationActive)
    {
        if (millis() - vibrationStartTime >= vibrationDuration)
        {
            digitalWrite(PIN_VIBRATION, LOW);
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
            digitalWrite(PIN_VIBRATION, HIGH);
        }
    }
}