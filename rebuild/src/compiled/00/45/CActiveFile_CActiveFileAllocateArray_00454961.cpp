#pragma optimize("s",on)
#include <stdlib.h>

void* __fastcall Copy_MemoryStepped(int first, int last, void* destination, const void* tag);

#pragma optimize("y", off)
void* __stdcall CActiveFile_AllocateArray(int elementCount, int first, int last)
{
    void* buffer;
    if (elementCount != 0) {
        buffer = malloc(elementCount << 2);
    } else {
        buffer = 0;
    }
    Copy_MemoryStepped(first, last, buffer, (const char*)&elementCount + 3);
    return buffer;
}
#pragma optimize("y", on)