#ifndef LOGGER_H
#define LOGGER_H

#include <esp_log.h>
#include <stdarg.h>

class Logger
{
public:
    static void init();
    static void logPostureData(float pitch, float roll, bool stateChanged, const char *action);
    
private:
    static const char *TAG;
};

#endif