#include <Arduino.h>
#include "PostureMonitor.h"
#include "Logger.h"

PostureMonitor monitor;

void setup()
{
    ESP_LOGI("Main", "Iniciando sistema...");
    monitor.initialize();
}

void loop()
{
    monitor.update();
    delay(50);
}