#pragma once
#include <stddef.h>
#include "fable_base_class.h"
#include "fable_carray.h"
#include "fable_counted_pointer.h"
class CEngineScene;
class CEngineSceneGrid;
class CEngineSceneMap : public CBaseClassNonCopyable {
public:
    void Update();
    unsigned char Area[0x10];
    unsigned long MaxLevels;
    CArrayVector<CCountedPointer<CEngineSceneGrid> > GridLevels;
    CEngineScene* Scene;
};
FABLE_STATIC_ASSERT(offsetof(CEngineSceneMap,GridLevels)==0x18);
FABLE_STATIC_ASSERT(sizeof(CEngineSceneMap)==0x28);
