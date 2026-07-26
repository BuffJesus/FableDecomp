#pragma once

#include "rebuild_abi.h"

void* FABLE_CDECL FableFileInstallerAllocate(fable_u32 size);

struct CFileInstaller
{
    fable_u8 unknown00[0xA0];

    CFileInstaller();

    __forceinline static void* operator new(unsigned int size)
    {
        return FableFileInstallerAllocate(size);
    }
};

FABLE_STATIC_ASSERT(sizeof(CFileInstaller) == 0xA0);

struct CCountedFileInstaller
{
    CFileInstaller* object;
    void* reference;

    void operator=(CFileInstaller* value);
};

FABLE_STATIC_ASSERT(sizeof(CCountedFileInstaller) == 0x08);

// The singleton is owned through the engine's counted-pointer wrapper. Keeping
// the wrapper visible here documents why the retail storage occupies two
// pointers even though Get() returns only the CFileInstaller object.
extern CCountedFileInstaller g_CFileInstallerSingleton_013CA818;

struct CFileInstallerSingleton
{
    static CFileInstaller& FABLE_FASTCALL Get();
};
