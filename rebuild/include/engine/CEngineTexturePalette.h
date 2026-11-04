#pragma once
#include <stddef.h>
#include "fable_carray.h"
#include "fable_string.h"
#include "fable_stlport_uninitialized.h"
class CGraphicDataBank;
class CDataInputStream;
class CEngineTexturePalette {
public:
    struct CPaletteEntry { unsigned long Index; CCharString Name; };
    CEngineTexturePalette(const CGraphicDataBank*,bool,unsigned long);
    void Load(CDataInputStream&);
    const CGraphicDataBank* Bank;
    CArrayVector<CPaletteEntry> Palette;
    long DefaultIndex;
    bool RetailVersion;
};
FABLE_STATIC_ASSERT(sizeof(CEngineTexturePalette)==0x18);
FABLE_STATIC_ASSERT(sizeof(CEngineTexturePalette::CPaletteEntry)==8);
