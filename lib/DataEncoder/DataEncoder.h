#ifndef DATA_ENCODER_H
#define DATA_ENCODER_H

#include <Arduino.h>

class DataEncoder
{
public:
    static String encode(float pitch, float roll, bool stateChanged, const char *action);
};

#endif
