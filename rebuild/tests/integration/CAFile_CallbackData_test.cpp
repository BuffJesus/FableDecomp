#include "fable_filesystem.h"
#include <stdio.h>

static int errors;
static int hardDisks;
static const char* lastMessage;

void FABLE_FASTCALL OnError() { ++errors; }
void FABLE_FASTCALL OnHardDisk(const char* message)
{
    ++hardDisks;
    lastMessage = message;
}

int main()
{
    if (CAFile::m_errorCallback || CAFile::m_harddiskErrorCallback) return 1;
    for (int i = 0; i < 16; ++i) {
        const char* message = (i & 1) ? "disk one" : "disk two";
        CAFile::m_errorCallback = (i & 1) ? OnError : 0;
        CAFile::m_harddiskErrorCallback = (i & 2) ? OnHardDisk : 0;
        const int beforeError = errors;
        const int beforeDisk = hardDisks;
        CAFile::DoErrorCallback();
        CAFile::DoHardDiskErrorCallback(message);
        if (errors != beforeError + ((i & 1) != 0) ||
            hardDisks != beforeDisk + ((i & 2) != 0)) return 2;
        if ((i & 2) && lastMessage != message) return 3;
    }
    puts("CAFILE_CALLBACK_DATA_PASS cases=16");
    return 0;
}
