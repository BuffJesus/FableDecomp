#include "engine/CEngineMap.h"
#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CEngineLocalDetailGenerator.h"
#include "fable_engine_internal.h"
CEngineMap::~CEngineMap() {
    CloseStaticMap();
    if(LandscapeMap)NEngineInternal::GPLandscapeRenderer->DeleteMap(this);
    if(LocalDetailCacheMap)NEngineInternal::GPLocalDetailGenerator->DeleteMap(this);
    FileBlockManager.Reset();
}
