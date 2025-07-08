/**
 * 
 * 
 */

#pragma once

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#define MAX_SEND_LENGTH 256

typedef struct Send Send;

struct Send
{
    bool (*write_str)(const char* data);
    bool (*fwrite)(Send* send, const char* data, ...);
};

void SendInit(Send* sender, bool (*write_func)(const char* data));
bool SendFormatWrite(Send* send, const char* data, ...);
