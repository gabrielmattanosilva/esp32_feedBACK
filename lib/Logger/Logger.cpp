#include "Logger.h"

const char *Logger::TAG = "PostureMonitor";

void Logger::init()
{
    esp_log_level_set(TAG, ESP_LOG_INFO);
}

void Logger::logPostureData(float pitch, float roll, bool stateChanged, const char *action)
{
    ESP_LOGI(TAG, "%.1f | %.1f | %d | %s", pitch, roll, stateChanged ? 1 : 0, action);
}

void Logger::logMessage(esp_log_level_t level, const char *tag, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    esp_log_writev(level, tag, format, args);
    va_end(args);
}