#pragma once
#include "rebuild_abi.h"
class CMemoryDataInputStream;
// Ego_r.pdb and retail B6D6E0: the non-sea water header is eight bytes.
class CWaterStaticMapInfoBlock {
public:
    bool Load(CMemoryDataInputStream& stream,long size);
    unsigned long WaterStaticMapVersionNumber;
    unsigned long DataStreamPosition;
};
FABLE_STATIC_ASSERT(sizeof(CWaterStaticMapInfoBlock)==8);
