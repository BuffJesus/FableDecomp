#include "fable_system.h"

#include <string.h>

namespace
{
    const fable_u32 kDefaultDisplayWidth = 128;
    const fable_u32 kDefaultDisplayHeight = 64;
    const fable_u32 kDefaultMemoryBudget = 0x00040000UL;
    const fable_u32 kRegionAllocationSize = 0x00020000UL;
    const char kRegionGuid[] = "BBB GUID";
    const wchar_t kPrimaryIdentifier[] = L"b";
    const wchar_t kSecondaryIdentifier[] = L"C";
}

CSystemManagerInit::CSystemManagerInit()
{
    displayHeight = kDefaultDisplayHeight;
    displayWidth = kDefaultDisplayWidth;
    displayDepth = 64.0f;
    nearPlane = 2.0f;
    scale = 1.0f;
    unknown50 = 0;
    unknown4C = 0;
    memoryBudget = kDefaultMemoryBudget;

    unknown58 = 0;
    unknown59 = 0;
    unknown5A = 0;
    reinterpret_cast<CRegionDef*>(regionStorage)->ConstructStorage();

    CWideString* primaryIdentifier =
        reinterpret_cast<CWideString*>(&primaryIdentifierStorage);
    FableConstructWideString(primaryIdentifier);

    CWideString* secondaryIdentifier =
        reinterpret_cast<CWideString*>(&secondaryIdentifierStorage);
    FableConstructWideString(secondaryIdentifier);

    *reinterpret_cast<fable_u32*>(&unknown00[0]) = 0;
    *reinterpret_cast<fable_u32*>(&unknown00[4]) = 0;
    enabled = 1;
    unknown14 = 0;

    *reinterpret_cast<fable_u32*>(&regionStorage[0x40]) = 0;
    unknown11 = 0;
    memset(&regionStorage[0x44], 0, 0x11);
    strncpy(
        reinterpret_cast<char*>(&regionStorage[0x44]),
        kRegionGuid,
        8);
    regionStorage[0x4C] = '\0';

    *primaryIdentifier = kPrimaryIdentifier;

    *reinterpret_cast<fable_u32*>(&regionStorage[0x58]) =
        kRegionAllocationSize;

    unknown19 = 0;
    unknown20 = 0;
    unknown1C = 0;
    unknown21 = 0;
    unknown24 = 0;
    unknown28 = 0;

    *secondaryIdentifier = kSecondaryIdentifier;

    unknownC0 = 0;
    unknownC4 = 0;
    useDefaultDisplay = 1;
}
