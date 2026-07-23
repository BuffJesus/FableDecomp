#ifndef FABLETLC_BUILD_NUMBER_COMPILED_H
#define FABLETLC_BUILD_NUMBER_COMPILED_H

#include <stddef.h>
#include "rebuild_abi.h"

class CAFile {};

class CWideString
{
public:
    bool PathExists() const;
    fable_u32 Opaque;
};

class CCharString
{
public:
    void AssignFromWide(const wchar_t* text);
    void ConstructFromFile(CAFile* file, int length);
    CCharString* Assign(const CCharString& other);
    void Destroy();
    fable_u32 Opaque;
};

#pragma pack(push, 1)
class CDiskFileWin32
{
public:
    void Construct(const CWideString& path, int accessMode, int shareMode);
    void Close();
    void BaseCleanup();

    void** Vftable;
    fable_u8 Unknown04[0x14];
    CCharString Path;
};
#pragma pack(pop)

extern CCharString FableFullBuildVersion;
extern void* FableDiskFileWin32Vftable[];

void FABLE_CDECL GFGetBuildNumber2();

FABLE_STATIC_ASSERT(sizeof(CWideString) == 0x04);
FABLE_STATIC_ASSERT(sizeof(CCharString) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CDiskFileWin32, Vftable) == 0x00);
FABLE_STATIC_ASSERT(offsetof(CDiskFileWin32, Path) == 0x18);
FABLE_STATIC_ASSERT(sizeof(CDiskFileWin32) == 0x1C);

#endif
