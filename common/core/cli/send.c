
#include "send.h"

void SendInit(Send* sender, bool (*write_func)(const char* data))
{
    sender->write_str = write_func;
    sender->fwrite = SendFormatWrite;
}

bool SendFormatWrite(Send* send, const char* data, ...)
{
    char buf[MAX_SEND_LENGTH] = {0};
    va_list vl;
    va_start(vl, data);
    vsnprintf(buf, MAX_SEND_LENGTH, data, vl);
    va_end(vl);

    buf[MAX_SEND_LENGTH - 1] = '\0';

    return send->write_str(buf);
}
