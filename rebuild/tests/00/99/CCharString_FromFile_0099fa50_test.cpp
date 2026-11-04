#include <new>
#include <stdio.h>
#include "fable_string.h"

fable_i32 g_CCharStringInstanceCount_013BD800 = 0;
static CCharString* receiver;
static CAFile* receivedFile;
static CASuspendableProcess* receivedProcess;
static long receivedLength;
static int loads;
static int unassigns;

void CCharString::LoadFromFile(
    CAFile& file, CASuspendableProcess* process, long length)
{
    receiver = this;
    receivedFile = &file;
    receivedProcess = process;
    receivedLength = length;
    ++loads;
}
void CCharString::UnassignString() { ++unassigns; }
CCharString::~CCharString() { --g_CCharStringInstanceCount_013BD800; }

int main()
{
    unsigned char fileBytes[8] = {0};
    CAFile* file = reinterpret_cast<CAFile*>(fileBytes);
    const long lengths[] = {-1, 0, 7};
    for (int i = 0; i < 3; ++i)
    {
        unsigned char raw[sizeof(CCharString)];
        CCharString* text = new (raw) CCharString(file, lengths[i]);
        if (g_CCharStringInstanceCount_013BD800 != 1 ||
            *reinterpret_cast<void**>(raw) != 0 || unassigns != 0)
            return 1;
        if (loads != i + 1 || receiver != text || receivedFile != file ||
            receivedProcess != 0 || receivedLength != lengths[i])
            return 2;
        text->~CCharString();
        if (g_CCharStringInstanceCount_013BD800 != 0) return 3;
    }
    puts("CHARSTRING_FROM_FILE_PASS");
    return 0;
}
