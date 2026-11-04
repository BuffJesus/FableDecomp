#include "engine/CCompressorZlib.h"
#include <stdlib.h>
void* __cdecl CCompressorZlib::Alloc(void*,unsigned int count,unsigned int size)
{
    __int64 bytes = (unsigned __int64)count * size;
    if (bytes != 0 && (__int64)bytes >= 0 && (__int64)bytes <= 0x7fffffff) return malloc(count * size);
    return 0;
}