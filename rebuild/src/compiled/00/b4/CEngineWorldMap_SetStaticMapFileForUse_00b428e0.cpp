#include "engine/CEngineWorldMap.h"
#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CEngineLandscapeMemoryManager.h"
#include "fable_engine_internal.h"
#include "fable_progress_display.h"

void CEngineWorldMap::SetStaticMapFileForUse(const CWideString& path)
{
    { CCharString text("SetStaticMapFileForUse: CloseStaticMapFile"); NProgressDisplay::DisplayProgress(text,-1.0f,false,false); }
    CloseStaticMapFile();
    { CCharString text("SetStaticMapFileForUse: EnablePoolAllocation"); NProgressDisplay::DisplayProgress(text,-1.0f,false,false); }
    NEngineInternal::GPLandscapeRenderer->MemoryManager->EnablePoolAllocation(true);
    StaticMapBankName=path;
    { CCharString text("SetStaticMapFileForUse: OpenStaticMaps"); NProgressDisplay::DisplayProgress(text,-1.0f,false,false); }
    OpenStaticMaps(CEngineMap::STATIC_MAP_OPEN_STATE_OPEN_FOR_USE);
    StaticMapBuildStatus=3;
    { CCharString text("SetStaticMapFileForUse: LoadWaterData"); NProgressDisplay::DisplayProgress(text,-1.0f,false,false); }
    LoadWaterData();
}
