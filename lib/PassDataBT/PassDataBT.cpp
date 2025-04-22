#include "PassDataBT.h"

BluetoothSerial PassDataBT::bt;
String PassDataBT::buffer = "";

void PassDataBT::begin(const char *deviceName)
{
    bt.begin(deviceName);
}

void PassDataBT::bufferAndSend(const String &encodedLine)
{
    // Adiciona ao buffer com separador
    if (!buffer.isEmpty())
    {
        buffer += ";";
    }
    buffer += encodedLine;

    // Conta quantos dados estão no buffer
    int count = 1;
    for (char c : buffer)
    {
        if (c == ';')
            count++;
    }

    // Se atingir limite, envia o lote
    if (count >= maxBufferSize)
    {
        flushBuffer();
    }
}

void PassDataBT::flushBuffer()
{
    bt.println(buffer);
    buffer = "";
}
