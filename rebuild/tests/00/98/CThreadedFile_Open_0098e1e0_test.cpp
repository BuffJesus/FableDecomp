#include <stdio.h>
#include <string.h>

#include "fable_string.h"
#include "fable_threaded_file.h"

extern "C" fable_u8 g_FableThreadedEmptyChar_0122D70E = 0;
extern "C" char g_FableThreadedDDrive_0129A15C[] = "D:";
extern "C" fable_u32 g_FableThreadedPhysicalSortKey_013BC9EC = 0;
extern "C" void* g_FableThreadedCreateFileW_0143FE2C = 0;
extern "C" void* g_FableThreadedGetFileSize_0143FDF0 = 0;

namespace
{
    struct CharData
    {
        char* text;
    };

    const wchar_t g_fullPath[] = L"D:\\banks\\text.big";
    CharData g_driveData;
    fable_u8 g_nameBytes[sizeof(CWideString)];
    bool g_driveD;
    bool g_createSucceeds;
    bool g_argumentsValid;
    unsigned int g_destroyCalls;
    unsigned int g_assignCalls;
    unsigned int g_getSizeCalls;
    unsigned long g_lastFlags;
    void* const g_handle = reinterpret_cast<void*>(0x1234);

    CThreadedFile* RawFile(fable_u8* bytes)
    {
        return reinterpret_cast<CThreadedFile*>(bytes);
    }

    const CWideString& Name()
    {
        return *reinterpret_cast<const CWideString*>(g_nameBytes);
    }

    void Reset(bool driveD, bool createSucceeds)
    {
        g_driveD = driveD;
        g_createSucceeds = createSucceeds;
        g_argumentsValid = true;
        g_destroyCalls = 0;
        g_assignCalls = 0;
        g_getSizeCalls = 0;
        g_lastFlags = 0;
        g_driveData.text = const_cast<char*>(driveD ? "D:" : "C:");
        memset(g_nameBytes, 0, sizeof(g_nameBytes));
        g_FableThreadedPhysicalSortKey_013BC9EC = 40;
    }

    bool CheckOpen(bool driveD, bool noCaching)
    {
        Reset(driveD, true);
        fable_u8 fileBytes[sizeof(CThreadedFile)];
        memset(fileBytes, 0, sizeof(fileBytes));
        CThreadedFile* file = RawFile(fileBytes);

        const bool result = file->Open(Name(), noCaching);
        return
            result &&
            g_argumentsValid &&
            file->deviceId_ == (driveD ? 1U : 0U) &&
            file->fileHandle_ == g_handle &&
            file->filenameStorage_ == g_nameBytes &&
            file->length_ == 0x345678 &&
            file->physicalSortKey_ == 41 &&
            !file->openedForWrite_ &&
            file->openFlag_ &&
            g_FableThreadedPhysicalSortKey_013BC9EC == 41 &&
            g_lastFlags ==
                (noCaching ? 0x60000001UL : 0x40000001UL) &&
            g_assignCalls == 1 &&
            g_getSizeCalls == 1 &&
            g_destroyCalls == 4;
    }

    bool CheckFailure()
    {
        Reset(false, false);
        fable_u8 fileBytes[sizeof(CThreadedFile)];
        memset(fileBytes, 0, sizeof(fileBytes));
        CThreadedFile* file = RawFile(fileBytes);

        const bool result = file->Open(Name(), false);
        return
            !result &&
            g_argumentsValid &&
            file->fileHandle_ == reinterpret_cast<void*>(-1) &&
            !file->openedForWrite_ &&
            !file->openFlag_ &&
            g_assignCalls == 0 &&
            g_getSizeCalls == 0 &&
            g_FableThreadedPhysicalSortKey_013BC9EC == 40 &&
            g_destroyCalls == 4;
    }
}

extern "C" void FABLE_FASTCALL
FableThreadedConvertFullPath_0098E1E0(
    void* result,
    const void* pathname)
{
    g_argumentsValid =
        g_argumentsValid &&
        pathname == g_nameBytes;
    *reinterpret_cast<const wchar_t**>(result) = g_fullPath;
}

extern "C" void FABLE_FASTCALL
FableThreadedCharConstruct_0098E1E0(void* string, void*)
{
    *reinterpret_cast<void**>(string) = 0;
}

extern "C" void FABLE_FASTCALL
FableThreadedSplitPath_0098E1E0(
    const void*,
    void*,
    void*,
    void*,
    void*)
{
}

extern "C" void* FABLE_FASTCALL
FableThreadedConvertDriveToChar_0098E1E0(
    void* temporary,
    void*,
    void*,
    void* result)
{
    *reinterpret_cast<CharData**>(result) = &g_driveData;
    *reinterpret_cast<CharData**>(temporary) = &g_driveData;
    return result;
}

extern "C" void* FABLE_FASTCALL
FableThreadedCharToUpper_0098E1E0(void* string, void*)
{
    return string;
}

extern "C" bool FABLE_FASTCALL
FableThreadedCharEquals_0098E1E0(
    const void*,
    void*,
    const char* literal)
{
    g_argumentsValid =
        g_argumentsValid &&
        literal == g_FableThreadedDDrive_0129A15C;
    return g_driveD;
}

extern "C" void FABLE_FASTCALL
FableThreadedCharDestroy_0098E1E0(void*, void*)
{
    ++g_destroyCalls;
}

extern "C" const wchar_t* FABLE_FASTCALL
FableThreadedWideConversion_0098E1E0(const void*, void*)
{
    return g_fullPath;
}

extern "C" void FABLE_FASTCALL
FableThreadedWideAssign_0098E1E0(
    void* destination,
    void*,
    const void* source)
{
    ++g_assignCalls;
    g_argumentsValid =
        g_argumentsValid &&
        destination != 0 &&
        source == g_nameBytes;
    *reinterpret_cast<const void**>(destination) = source;
}

extern "C" void* __stdcall
FableThreadedCreateFileStub_0098E1E0(
    const wchar_t* filename,
    unsigned long desiredAccess,
    unsigned long shareMode,
    void*,
    unsigned long creationDisposition,
    unsigned long flags,
    void*)
{
    g_lastFlags = flags;
    g_argumentsValid =
        g_argumentsValid &&
        filename == g_fullPath &&
        desiredAccess == 0x80000000 &&
        shareMode == 1 &&
        creationDisposition == 3;
    return
        g_createSucceeds ?
            g_handle :
            reinterpret_cast<void*>(-1);
}

extern "C" unsigned long __stdcall
FableThreadedGetFileSizeStub_0098E1E0(
    void* handle,
    unsigned long*)
{
    ++g_getSizeCalls;
    g_argumentsValid =
        g_argumentsValid &&
        handle == g_handle;
    return 0x345678;
}

int main()
{
    g_FableThreadedCreateFileW_0143FE2C =
        FableThreadedCreateFileStub_0098E1E0;
    g_FableThreadedGetFileSize_0143FDF0 =
        FableThreadedGetFileSizeStub_0098E1E0;

    const bool passed =
        CheckOpen(false, false) &&
        CheckOpen(true, true) &&
        CheckFailure();
    if (!passed)
    {
        printf("FABLETLC_THREADED_FILE_OPEN FAIL\n");
        return 1;
    }

    printf("FABLETLC_THREADED_FILE_OPEN PASS\n");
    return 0;
}
