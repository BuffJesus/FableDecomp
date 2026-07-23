#ifndef FABLETLC_OBJECT_FAMILY_COMPILED_H
#define FABLETLC_OBJECT_FAMILY_COMPILED_H

#include <stddef.h>
#include "rebuild_abi.h"

struct CObjectFamilyEntry
{
    long Object;
    long Probability;
};

class CMainGameComponent
{
public:
    fable_u8 pad_0000[0x1613C];
    unsigned long WorldSeed;
};

extern CMainGameComponent* gMainGameComponent;
unsigned long __fastcall GFROR13(unsigned long value);

class CObjectFamilyDef
{
public:
    fable_u8 pad_0000[0x3C];
    CObjectFamilyEntry* mObjectsBegin;
    CObjectFamilyEntry* mObjectsEnd;
    CObjectFamilyEntry* mObjectsCapacity;

    long GetRandomObject() const;
};

FABLE_STATIC_ASSERT(sizeof(CObjectFamilyEntry) == 8);
FABLE_STATIC_ASSERT(offsetof(CObjectFamilyDef, mObjectsBegin) == 0x3C);
FABLE_STATIC_ASSERT(offsetof(CMainGameComponent, WorldSeed) == 0x1613C);

#endif
