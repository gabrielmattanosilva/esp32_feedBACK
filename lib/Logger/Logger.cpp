#include "Logger.h"
#include "DataEncoder.h"
//#include "PassDataBT.h"

const char *Logger::TAG = "PostureMonitor";

void Logger::init()
{
    Serial.begin(115200);
    esp_log_level_set(TAG, ESP_LOG_INFO);
    //PassDataBT::begin();
}

void Logger::logPostureData(float pitch, float roll, bool stateChanged, const char *action)
{
    ESP_LOGI(TAG, "%.1f | %.1f | %d | %s", pitch, roll, stateChanged ? 1 : 0, action);

    // Extrair parte principal da ação
    String actionStr(action);
    int sepIndex = actionStr.indexOf(" | ");
    if (sepIndex != -1) {
        actionStr = actionStr.substring(0, sepIndex);
    }

    // Codifica os dados
    String encoded = DataEncoder::encode(pitch, roll, stateChanged, actionStr.c_str());

    // Mostra codificado no serial
    Serial.println(encoded);

    // Envia para o app via Bluetooth (em lote)
    //PassDataBT::bufferAndSend(encoded);
}
