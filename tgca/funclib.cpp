#include "funclib.h"

QByteArray cmdHead(int cmd, int dsz)
{
    QByteArray head;
    head.append((char*)&cmd, 4);
    head.append((char*)&dsz, 4);
    return head;
}
