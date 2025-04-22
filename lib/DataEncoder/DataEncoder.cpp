#include "DataEncoder.h"

String DataEncoder::encode(float pitch, float roll, bool stateChanged, const char *action)
{
    int actionCode = 0;
    String act(action);

    if (act == "Vibracao breve")
    {
        actionCode = 1;
    }
    else if (act.startsWith("Vibracao prolongada"))
    {
        actionCode = 2;
    }
    else
    {
        actionCode = 0; // "Nenhuma acao" ou qualquer outro
    }

    String encoded = String(pitch, 1) + "/" +
                     String(roll, 1) + "/" +
                     String(stateChanged ? 1 : 0) + "/" +
                     String(actionCode);
    return encoded;
}
