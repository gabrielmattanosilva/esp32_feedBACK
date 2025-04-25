#include <Arduino.h>
#include "PostureMonitor.h"
#include "Logger.h"

PostureMonitor monitor;

void setup()
{
    ESP_LOGI("MAIN", "Iniciando sistema...");
    monitor.initialize();
}

void loop()
{
    monitor.update();
    delay(50); // 20Hz
}