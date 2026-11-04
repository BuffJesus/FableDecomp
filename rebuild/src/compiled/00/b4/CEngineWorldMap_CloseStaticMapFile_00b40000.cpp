#include "engine/CEngineWorldMap.h"
#include "engine/CEngineMap.h"
#include "engine/CEngineWaterRenderer.h"
#include "fable_engine_internal.h"
void CEngineWorldMap::CloseStaticMapFile() {
    if(StaticMapOpenState) {
        for(unsigned long i=1;i<MapList.size();++i) {
            CEngineMap* map=MapList[i];
            if(map)map->CloseStaticMap();
        }
        if(NEngineInternal::GPWaterRenderer)NEngineInternal::GPWaterRenderer->CloseStaticMap();
        StaticMapFile.Close();
        StaticMapOpenState=0;
        StaticMapBuildStatus=0;
    }
}
