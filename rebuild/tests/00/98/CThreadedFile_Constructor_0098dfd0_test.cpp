#include <new>
#include <stdio.h>
#include <string.h>

#include "fable_threaded_file.h"
#include "test_support/base_class_stubs.h"

namespace
{
    void* g_expectedFile;
    int g_step;
    int g_baseStep;
    int g_filenameStep;
    void* g_baseSelf;
    void* g_filenameSelf;
    unsigned int g_destructorCalls;
}

CBaseClassNonCopyable::CBaseClassNonCopyable()
{
    g_baseStep = ++g_step;
    g_baseSelf = this;
}

CBaseClassNonCopyable::~CBaseClassNonCopyable()
{
}

CWideString::CWideString()
{
    g_filenameStep = ++g_step;
    g_filenameSelf = this;
    storage_ = reinterpret_cast<CWideStringData*>(0x24681357);
}

CWideString::~CWideString()
{
}

CThreadedFile::~CThreadedFile()
{
    ++g_destructorCalls;
}

int main()
{
    fable_u8 storage[sizeof(CThreadedFile)];
    memset(storage, 0xA5, sizeof(storage));
    g_expectedFile = storage;

    CThreadedFile* file = new (storage) CThreadedFile;

    // The constructor must leave the CThreadedFile vtable installed.
    static_cast<CBaseClass*>(file)->~CBaseClass();

    const bool passed =
        file == reinterpret_cast<CThreadedFile*>(storage) &&
        g_baseStep == 1 &&
        g_baseSelf == storage &&
        g_filenameStep == 2 &&
        g_filenameSelf == storage + 0x0C &&
        g_destructorCalls == 1 &&
        file->fileHandle_ == reinterpret_cast<void*>(-1) &&
        *reinterpret_cast<fable_u32*>(storage + 0x0C) == 0x24681357 &&
        !file->openedForWrite_ &&
        !file->openFlag_ &&
        file->deviceId_ == 0xA5A5A5A5 &&
        file->length_ == 0xA5A5A5A5 &&
        file->physicalSortKey_ == 0xA5A5A5A5 &&
        storage[0x1A] == 0xA5 &&
        storage[0x1B] == 0xA5;

    if (!passed)
    {
        printf("FABLETLC_THREADED_FILE_CONSTRUCTOR FAIL\n");
        return 1;
    }

    printf("FABLETLC_THREADED_FILE_CONSTRUCTOR PASS\n");
    return 0;
}
