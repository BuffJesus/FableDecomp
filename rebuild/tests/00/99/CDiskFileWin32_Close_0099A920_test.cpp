#include "rebuild_abi.h"

#include <stdio.h>

#pragma pack(push, 1)
struct FableDiskFileWin32_0099A920
{
    void** vtable;
    fable_u32 accessType;
    fable_i32 position;
    fable_i32 length;
    void* winFileHandle;
    fable_u8 opened;
    fable_u8 padding15[3];
    void* pathnameStorage;
};
#pragma pack(pop)

extern "C" void FABLE_FASTCALL
FableDiskFileWin32Close_0099A920(
    FableDiskFileWin32_0099A920* self,
    void*);

namespace
{
    unsigned int g_seekCalls;
    unsigned int g_flushCalls;
    unsigned int g_closeCalls;
    void* g_expectedHandle;

    unsigned long __stdcall SetFilePointerStub(
        void* handle,
        long distance,
        long* highDistance,
        unsigned long method)
    {
        if (
            handle != g_expectedHandle ||
            distance != 0 ||
            highDistance != 0 ||
            method != 1)
        {
            return 0xFFFFFFFFul;
        }
        ++g_seekCalls;
        return 0;
    }

    int __stdcall FlushFileBuffersStub(void* handle)
    {
        if (handle != g_expectedHandle)
        {
            return 0;
        }
        ++g_flushCalls;
        return 1;
    }

    int __stdcall CloseHandleStub(void* handle)
    {
        if (handle != g_expectedHandle)
        {
            return 0;
        }
        ++g_closeCalls;
        return 1;
    }
}

typedef unsigned long (__stdcall *FableSetFilePointerProc)(
    void*,
    long,
    long*,
    unsigned long);
typedef int (__stdcall *FableHandleProc)(void*);

extern "C" FableSetFilePointerProc
g_FableSetFilePointer_0143FDE8 = SetFilePointerStub;
extern "C" FableHandleProc
g_FableFlushFileBuffers_0143FDA8 = FlushFileBuffersStub;
extern "C" FableHandleProc
g_FableCloseHandle_0143FE30 = CloseHandleStub;

int main()
{
    FableDiskFileWin32_0099A920 file = {0};
    g_expectedHandle = reinterpret_cast<void*>(0x12345678);
    file.winFileHandle = g_expectedHandle;
    file.opened = 1;

    FableDiskFileWin32Close_0099A920(&file, 0);
    if (
        file.opened != 0 ||
        g_seekCalls != 1 ||
        g_flushCalls != 1 ||
        g_closeCalls != 1)
    {
        return 1;
    }

    FableDiskFileWin32Close_0099A920(&file, 0);
    if (
        file.opened != 0 ||
        g_seekCalls != 1 ||
        g_flushCalls != 1 ||
        g_closeCalls != 1)
    {
        return 2;
    }

    printf("FABLETLC_DISK_FILE_CLOSE_0099A920 PASS\n");
    return 0;
}
