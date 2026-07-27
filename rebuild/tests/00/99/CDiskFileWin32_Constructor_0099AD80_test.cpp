#include "rebuild_abi.h"

#include <stdio.h>
#include <string.h>

struct FableWideString_0099AD80
{
    void* storage;
};

#pragma pack(push, 1)
struct FableDiskFileWin32_0099AD80
{
    void** vtable;
    fable_u32 accessType;
    fable_i32 position;
    fable_i32 length;
    void* winFileHandle;
    fable_u8 opened;
    fable_u8 padding15[3];
    FableWideString_0099AD80 pathname;
};
#pragma pack(pop)

extern "C" FableDiskFileWin32_0099AD80* FABLE_FASTCALL
FableConstructDiskFileWin32_0099AD80(
    FableDiskFileWin32_0099AD80* self,
    void*,
    const FableWideString_0099AD80* pathname,
    fable_u32 accessType,
    fable_u32 flags);

namespace
{
    unsigned int g_baseConstructCalls;
    unsigned int g_wideConstructCalls;
    unsigned int g_openCalls;
    FableDiskFileWin32_0099AD80* g_openSelf;
    const FableWideString_0099AD80* g_openPathname;
    fable_u32 g_openAccessType;
    fable_u32 g_openFlags;
}

extern "C" void* g_CDiskFileWin32VTable_0122D06C[1] = {
    reinterpret_cast<void*>(0x122D06C)};

extern "C" void FABLE_FASTCALL
FableCAFileConstruct_0099A2F0(
    FableDiskFileWin32_0099AD80* self,
    void*)
{
    ++g_baseConstructCalls;
    memset(self, 0xA5, sizeof(*self));
}

extern "C" void FABLE_FASTCALL
FableWideStringConstructForDisk_0099AED0(
    FableWideString_0099AD80* self,
    void*)
{
    ++g_wideConstructCalls;
    self->storage = 0;
}

extern "C" void FABLE_FASTCALL
FableDiskFileOpen_0099A6A0(
    FableDiskFileWin32_0099AD80* self,
    void*,
    const FableWideString_0099AD80* pathname,
    fable_u32 accessType,
    fable_u32 flags)
{
    ++g_openCalls;
    g_openSelf = self;
    g_openPathname = pathname;
    g_openAccessType = accessType;
    g_openFlags = flags;
    self->accessType = accessType;
    self->opened = 1;
}

int main()
{
    FableWideString_0099AD80 pathname;
    pathname.storage = reinterpret_cast<void*>(0x13572468);

    FableDiskFileWin32_0099AD80 file;
    FableDiskFileWin32_0099AD80* result =
        FableConstructDiskFileWin32_0099AD80(
            &file,
            0,
            &pathname,
            3,
            0x240);

    if (
        result != &file ||
        file.vtable != g_CDiskFileWin32VTable_0122D06C ||
        file.pathname.storage != 0 ||
        file.accessType != 3 ||
        file.opened != 1 ||
        g_baseConstructCalls != 1 ||
        g_wideConstructCalls != 1 ||
        g_openCalls != 1 ||
        g_openSelf != &file ||
        g_openPathname != &pathname ||
        g_openAccessType != 3 ||
        g_openFlags != 0x240)
    {
        return 1;
    }

    printf("FABLETLC_DISK_FILE_CTOR_0099AD80 PASS\n");
    return 0;
}
