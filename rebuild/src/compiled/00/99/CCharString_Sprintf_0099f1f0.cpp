#include "fable_string.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

void CCharString::Sprintf(const char* format, ...)
{
    unsigned long capacity = strlen(format) + 0x400;
    char* buffer = capacity ? static_cast<char*>(malloc(capacity)) : 0;
    memset(buffer, 0, capacity);

    va_list args;
    va_start(args, format);
    int count = _vsnprintf(buffer, capacity, format, args);
    va_end(args);
    buffer[capacity - 1] = '\0';

    if (count <= static_cast<int>(capacity))
    {
        if (storage_ != 0)
            UnassignString();
        if (buffer != 0 && buffer[0] != '\0')
            storage_ = new CCharStringData(buffer);
    }
    if (buffer != 0)
        free(buffer);
}
