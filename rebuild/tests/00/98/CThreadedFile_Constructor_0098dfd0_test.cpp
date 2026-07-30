#include <new>
#include <stdio.h>
#include <string.h>

#include "fable_threaded_file.h"

extern "C" fable_u8 g_FableThreadedFileVTable_0129A158 = 0;

namespace
{
    void* g_expectedFile;
    bool g_baseCalled;
    bool g_pathCalled;
    bool g_argumentsValid;
}

extern "C" void FABLE_FASTCALL
FableThreadedFileBaseConstruct_0098DFD0(
    void* file,
    void*)
{
    g_baseCalled = true;
    g_argumentsValid =
        g_argumentsValid &&
        file == g_expectedFile;
}

extern "C" void FABLE_FASTCALL
FableThreadedFilePathConstruct_0098DFD0(
    void* pathname,
    void*)
{
    g_pathCalled = true;
    g_argumentsValid =
        g_argumentsValid &&
        pathname == reinterpret_cast<fable_u8*>(g_expectedFile) + 0x0C;
    *reinterpret_cast<fable_u32*>(pathname) = 0x24681357;
}

int main()
{
    fable_u8 storage[sizeof(CThreadedFile)];
    memset(storage, 0xA5, sizeof(storage));
    g_expectedFile = storage;
    g_baseCalled = false;
    g_pathCalled = false;
    g_argumentsValid = true;

    CThreadedFile* file = new (storage) CThreadedFile;

    const bool passed =
        file == reinterpret_cast<CThreadedFile*>(storage) &&
        g_baseCalled &&
        g_pathCalled &&
        g_argumentsValid &&
        file->vtable_ ==
            reinterpret_cast<fable_u32>(
                &g_FableThreadedFileVTable_0129A158) &&
        file->fileHandle_ == reinterpret_cast<void*>(-1) &&
        file->filenameStorage_ ==
            reinterpret_cast<void*>(0x24681357) &&
        !file->openedForWrite_ &&
        !file->openFlag_ &&
        file->deviceId_ == 0xA5A5A5A5 &&
        file->length_ == 0xA5A5A5A5 &&
        file->physicalSortKey_ == 0xA5A5A5A5;

    if (!passed)
    {
        printf("FABLETLC_THREADED_FILE_CONSTRUCTOR FAIL\n");
        return 1;
    }

    printf("FABLETLC_THREADED_FILE_CONSTRUCTOR PASS\n");
    return 0;
}
