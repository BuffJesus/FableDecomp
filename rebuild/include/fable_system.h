#pragma once

#include <stddef.h>

#include "fable_string.h"

#pragma pack(push, 1)
class CRegionDef
{
public:
    CRegionDef();
    void ConstructStorage();

    fable_u8 unknown00[0x44];
    char guid[0x11];
    fable_u8 unknown55[0x03];
    fable_u32 allocationSize;
};

class CSystemManagerInit
{
public:
    CSystemManagerInit();

    fable_u8 unknown00[0x0C];
    fable_u32 startupTextStorage;
    fable_u8 enabled;
    fable_u8 unknown11;
    fable_u8 unknown12[0x02];
    fable_u32 unknown14;
    fable_u8 unknown18;
    fable_u8 unknown19;
    fable_u8 unknown1A[0x02];
    fable_u32 unknown1C;
    fable_u8 unknown20;
    fable_u8 unknown21;
    fable_u8 unknown22[0x02];
    fable_u32 unknown24;
    fable_u32 unknown28;
    fable_u32 optionNameAStorage;
    fable_u32 optionNameBStorage;
    fable_u8 useDefaultDisplay;
    fable_u8 unknown35[0x03];
    fable_u32 displayWidth;
    fable_u32 displayHeight;
    float displayDepth;
    float nearPlane;
    float scale;
    fable_u32 unknown4C;
    fable_u32 unknown50;
    fable_u32 memoryBudget;
    fable_u8 unknown58;
    fable_u8 unknown59;
    fable_u8 unknown5A;
    fable_u8 unknown5B;
    fable_u8 regionStorage[sizeof(CRegionDef)];
    fable_u32 primaryIdentifierStorage;
    fable_u32 secondaryIdentifierStorage;
    fable_u32 unknownC0;
    fable_u32 unknownC4;
};
#pragma pack(pop)

enum ESystemCode
{
    kSystemCodeSuccess = 1,
    kSystemCodeWindowInitialisationFailed = 4
};

void FABLE_FASTCALL FableConstructWideString(CWideString* value);
void FABLE_FASTCALL FableConstructCharString(CCharString* value);
const CWideString& FABLE_FASTCALL FableAssignWideString(
    CWideString* value,
    void* unused,
    const wchar_t* text);
CSystemManagerInit* FABLE_FASTCALL
FableGetSystemManagerInitBoundary();

FABLE_STATIC_ASSERT(offsetof(CRegionDef, guid) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CRegionDef, allocationSize) == 0x58);
FABLE_STATIC_ASSERT(sizeof(CRegionDef) == 0x5C);
FABLE_STATIC_ASSERT(offsetof(CSystemManagerInit, startupTextStorage) == 0x0C);
FABLE_STATIC_ASSERT(offsetof(CSystemManagerInit, optionNameAStorage) == 0x2C);
FABLE_STATIC_ASSERT(offsetof(CSystemManagerInit, regionStorage) == 0x5C);
FABLE_STATIC_ASSERT(offsetof(CSystemManagerInit, primaryIdentifierStorage) == 0xB8);
FABLE_STATIC_ASSERT(sizeof(CSystemManagerInit) == 0xC8);
