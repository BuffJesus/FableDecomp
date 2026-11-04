#pragma once
#include <stddef.h>
#include "fable_base_class.h"
#include "fable_carray.h"
class CEngineSceneGridCell;
class CEngineSceneGrid : public CBaseClassNonCopyable {
public:
    void InvalidateBoundingBoxAndDrawDistance(unsigned long,unsigned long,bool);
    unsigned long Level;
    CEngineSceneGrid* LevelUp;
    CEngineSceneGrid* LevelDown;
    unsigned long Size;
    CArrayVector<CEngineSceneGridCell*> GridCells;
};
FABLE_STATIC_ASSERT(offsetof(CEngineSceneGrid,LevelDown)==0xc);
FABLE_STATIC_ASSERT(offsetof(CEngineSceneGrid,GridCells)==0x14);
FABLE_STATIC_ASSERT(sizeof(CEngineSceneGrid)==0x20);
