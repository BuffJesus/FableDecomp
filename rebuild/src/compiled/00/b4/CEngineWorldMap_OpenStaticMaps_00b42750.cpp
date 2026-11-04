#include "engine/CEngineWorldMap.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CEngineWaterRenderer.h"
#include "fable_engine_internal.h"
#include "fable_progress_display.h"

extern CCharString SeaStaticMapBankFileName;
extern CCharString CommonStaticMapHeaderEntryName;

template<> inline CCountedPointer<CMovableResourceMemoryPool>::CCountedPointer(const CCountedPointer& other)
    : Data(other.Data),Info(other.Info)
{
    if(Info) ++Info->RefCount;
}

void CEngineWorldMap::OpenStaticMaps(CEngineMap::STATIC_MAP_OPEN_STATE state)
{
    if(StaticMapOpenState==state) return;
    AutoBuildWorldMapGrid();
    if(state==CEngineMap::STATIC_MAP_OPEN_STATE_OPEN_FOR_UPDATE) {
        for(unsigned i=0;i<MapList.size();++i) {
            if(MapList[i]) {
                { CCharString text("SetStaticMapFileForUse: OpenStaticMap"); NProgressDisplay::DisplayProgress(text,-1.0f,false,false); }
                MapList[i]->OpenDevelopmentStaticMap(StaticMapFile,CEngineMap::STATIC_MAP_OPEN_STATE_OPEN_FOR_UPDATE);
            }
        }
    } else {
        bool opened=StaticMapFile.OpenFileReadOnly(StaticMapBankName,0x62);
        StaticMapFile.SetLoadingMemoryPool(NEngineInternal::GPResourceManager->LoadingMemoryPool);
        if(!opened) return;
        NEngineInternal::GPWaterRenderer->OpenStaticMapForUse(&StaticMapFile,SeaStaticMapBankFileName);
        long index=StaticMapFile.FindIndexBySymbol(CommonStaticMapHeaderEntryName);
        if(index>0) OpenStaticMapsUsingCommonRetailHeader(index);
        else {
            for(unsigned i=0;i<MapList.size();++i) {
                if(MapList[i]) {
                    { CCharString text("SetStaticMapFileForUse: OpenStaticMap"); NProgressDisplay::DisplayProgress(text,-1.0f,false,false); }
                    MapList[i]->OpenDevelopmentStaticMap(StaticMapFile,state);
                }
            }
        }
    }
    StaticMapOpenState=state;
}
