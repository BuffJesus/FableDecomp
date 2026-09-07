#pragma optimize("s",on)
#include <stdlib.h>

void* __fastcall CActiveFile_CopyRange(int first, int last, void* destination, const void* tag);

#pragma optimize("y", off)
void* __stdcall CActiveFile_AllocateAndCopy(int elementCount, int first, int last)
{
    void* buffer;
    if (elementCount != 0) {
        buffer = malloc(elementCount << 2);
    } else {
        buffer = 0;
    }
    CActiveFile_CopyRange(first, last, buffer, (const char*)&elementCount + 3);
    return buffer;
}
#pragma optimize("y", on)