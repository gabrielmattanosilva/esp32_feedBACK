#include "FeedbackHandler.h"

FeedbackHandler::FeedbackHandler() {}

void FeedbackHandler::initialize()
{
    pinMode(VIBRATION_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    startupSequence();
}

void FeedbackHandler::update()
{
    // Atualiza vibração
    if (vibrationActive)
    {
        if (millis() - vibrationStartTime >= vibrationDuration)
        {
            digitalWrite(VIBRATION_PIN, LOW);
            vibrationCount--;

            if (vibrationCount > 0)
            {
                vibrationStartTime = millis() + 200; // Intervalo entre vibrações
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

    // Atualiza LED piscante
    if (millis() - lastBlinkTime >= 500)
    {   // Pisca a cada 500ms
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState);
        lastBlinkTime = millis();
    }
}

void FeedbackHandler::triggerVibration(int count, int duration)
{
    vibrationCount = count;
    vibrationDuration = duration;
    vibrationStartTime = millis();
    vibrationActive = true;
    lastVibrationTime = millis();
}

void FeedbackHandler::setLED(bool state)
{
    ledState = state;
    digitalWrite(LED_PIN, ledState);
}

void FeedbackHandler::blinkLED()
{
    // Implementado no método update()
}

void FeedbackHandler::startupSequence()
{
    for (int i = 0; i < 3; i++)
    {
        setLED(true);
        digitalWrite(VIBRATION_PIN, HIGH);
        delay(100); // Pequeno delay aceitável durante inicialização
        setLED(false);
        digitalWrite(VIBRATION_PIN, LOW);
        delay(100);
    }
}

void FeedbackHandler::blinkError(int count)
{
    for (int i = 0; i < count; i++)
    {
        setLED(true);
        delay(200);
        setLED(false);
        delay(200);
    }
}