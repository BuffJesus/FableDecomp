#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/99/CDiskFileWin32_Read_0099a960.cpp"

extern "C" const char CDiskFileWin32_NullDestination_0099a960[] =
    "Null dst parameter";
extern "C" const char CDiskFileWin32_ReadPastEnd_0099a960[] =
    "Cannot read past end of file";
extern "C" const char CDiskFileWin32_ReadFailure_0099a960[] =
    "Cannot read file";
extern "C" unsigned char CDiskFileWin32_ThrowInfo_0099a960[1] = { 0 };

struct TestDiskFile_0099a960
{
    unsigned long padding0;
    unsigned long padding4;
    unsigned long position;
    unsigned long size;
    void* handle;
};

static volatile int g_ReadCalls;
static volatile unsigned long g_Requested;
static volatile unsigned long g_BytesToReport;
static void* volatile g_Handle;
static void* volatile g_Destination;
static void* volatile g_Overlapped;
static volatile int g_ReadResult;
static volatile int g_ThrowCalls;
static volatile int g_ThrowCode;
static void* volatile g_ThrowInfoSeen;

extern "C" int __stdcall TestReadFile_0099a960(
    void* handle,
    void* destination,
    unsigned long requested,
    unsigned long* bytesRead,
    void* overlapped)
{
    ++g_ReadCalls;
    g_Handle = handle;
    g_Destination = destination;
    g_Requested = requested;
    g_Overlapped = overlapped;
    *bytesRead = g_BytesToReport;
    return g_ReadResult;
}

extern "C" void* CDiskFileWin32_ReadFileIAT_0099a960 =
    (void*)&TestReadFile_0099a960;

extern "C" void* __fastcall
CDiskFileWin32_ConstructErrorString_0099a960(
    void* destination,
    void*,
    const char* text,
    int)
{
    *(const char**)destination = text;
    return destination;
}

extern "C" void* __fastcall
CDiskFileWin32_CopyErrorString_0099a960(
    void* destination,
    void*,
    const void* source)
{
    *(const char**)destination = *(const char* const*)source;
    return destination;
}

extern "C" char* __fastcall
CDiskFileWin32_GetErrorText_0099a960(void* value, void*)
{
    return *(char**)value;
}

extern "C" void __fastcall
CDiskFileWin32_DestroyErrorString_0099a960(void*, void*)
{
}

extern "C" void __stdcall
CDiskFileWin32_ThrowError_0099a960(void* exceptionValue, void* throwInfo)
{
    ++g_ThrowCalls;
    g_ThrowCode = *(int*)exceptionValue;
    g_ThrowInfoSeen = throwInfo;
}

static void ResetProbe(unsigned long bytesToReport)
{
    g_ReadCalls = 0;
    g_Requested = 0;
    g_BytesToReport = bytesToReport;
    g_Handle = 0;
    g_Destination = 0;
    g_Overlapped = (void*)1;
    g_ReadResult = 1;
    g_ThrowCalls = 0;
    g_ThrowCode = 0;
    g_ThrowInfoSeen = 0;
}

int main()
{
    TestDiskFile_0099a960 file;
    unsigned char destination[16];
    file.padding0 = 0;
    file.padding4 = 0;
    file.position = 2;
    file.size = 20;
    file.handle = (void*)0x12345678;

    typedef void (__fastcall *ReadFunction)(
        void*,
        void*,
        void*,
        long,
        void*);
    ReadFunction volatile read = &CDiskFileWin32_Read_0099a960;

    ResetProbe(5);
    read(&file, 0, destination, 8, (void*)0xabcdef01);
    int failures = 0;
    if (g_ReadCalls != 1 ||
        g_Handle != file.handle ||
        g_Destination != destination ||
        g_Requested != 8 ||
        g_Overlapped != 0 ||
        g_ThrowCalls != 0 ||
        file.position != 7)
    {
        ++failures;
    }

    ResetProbe(9);
    file.position = 7;
    read(&file, 0, destination, 0, (void*)0xabcdef01);
    if (g_ReadCalls != 0 ||
        g_ThrowCalls != 0 ||
        file.position != 7)
    {
        ++failures;
    }

    ResetProbe(0);
    g_ReadResult = 0;
    file.position = 2;
    read(&file, 0, destination, 8, 0);
    if (g_ReadCalls != 1 ||
        g_ThrowCalls != 1 ||
        g_ThrowCode != 4 ||
        g_ThrowInfoSeen != CDiskFileWin32_ThrowInfo_0099a960 ||
        file.position != 2)
    {
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "DISK_READ_0099a960_TEST FAIL count=%d reads=%d position=%lu\n",
            failures,
            g_ReadCalls,
            file.position);
        return 1;
    }

    std::printf("DISK_READ_0099a960_TEST PASS\n");
    return 0;
}
