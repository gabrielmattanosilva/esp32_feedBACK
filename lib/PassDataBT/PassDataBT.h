#ifndef PASSDATABT_H
#define PASSDATABT_H

#include <BluetoothSerial.h>

class PassDataBT
{
public:
    static void begin(const char *deviceName = "FeedBack Device");
    static void bufferAndSend(const String &encodedLine); // Envia em lote

private:
    static BluetoothSerial bt;
    static String buffer;
    static const int maxBufferSize = 10;

    static void flushBuffer(); // Envia o buffer acumulado
};

#endif
