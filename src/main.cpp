#include <Arduino.h>
#include "PostureMonitor.h"
#include "Logger.h"

PostureMonitor monitor;

void setup()
{
    // Inicializa o logger antes de tudo
    Logger::init();

    // Delay para estabilização (opcional)
    delay(1000);

    ESP_LOGI("MAIN", "Iniciando sistema...");
    monitor.initialize();
}

void loop()
{
    monitor.update();
    delay(50);
}