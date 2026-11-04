// CWorldMap::GetRegion (const), retail 0x004FC180
// (FableWin ?GetRegion@CWorldMap@@QBEABVCRegion@@J@Z). The non-const twin
// ?GetRegion@CWorldMap@@QAEAAVCRegion@@J@Z has the identical body at 0x004FB490.
#include "fable_world_map.h"

const CRegion& CWorldMap::GetRegion(long index) const
{
    return Regions[index];
}