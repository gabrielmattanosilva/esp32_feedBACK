#include "FeedbackHandler.h"

FeedbackHandler::FeedbackHandler() {}

void FeedbackHandler::initialize()
{
    pinMode(PIN_VIBRATION, OUTPUT);
    digitalWrite(PIN_VIBRATION, LOW);
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
    vibrationStartTime = millis(); // Marca o início da primeira vibração
    vibrationActive = true;
    digitalWrite(PIN_VIBRATION, HIGH); // Começa imediatamente
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
        unsigned long currentTime = millis();

        if (currentTime - vibrationStartTime < vibrationDuration)
        {
            // Período de vibração
            digitalWrite(PIN_VIBRATION, HIGH);
        }
        else if (currentTime - vibrationStartTime < vibrationDuration + 200)
        {
            // Período de pausa entre vibrações
            digitalWrite(PIN_VIBRATION, LOW);
        }
        else
        {
            // Finalizou um ciclo completo (vibração + pausa)
            vibrationCount--;

            if (vibrationCount > 0)
            {
                // Prepara próxima vibração
                vibrationStartTime = currentTime;
            }
            else
            {
                // Finalizou todas as vibrações
                vibrationActive = false;
                digitalWrite(PIN_VIBRATION, LOW);
            }
        }
    }
}