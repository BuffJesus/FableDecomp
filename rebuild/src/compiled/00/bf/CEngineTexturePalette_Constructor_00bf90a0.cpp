#include <new>
#include "engine/CEngineTexturePalette.h"
#include "fable_stlport_vector_impl.h"

CEngineTexturePalette::CEngineTexturePalette(const CGraphicDataBank* bank,bool retail,unsigned long defaultIndex)
    : Bank(bank),DefaultIndex(defaultIndex),RetailVersion(retail)
{
    if(!RetailVersion) {
        CPaletteEntry entry;
        entry.Index=0;
        Palette.push_back(entry);
    }
}
