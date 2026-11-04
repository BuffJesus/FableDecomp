#include "engine/CEngineWorldMap.h"
#include "engine/CEngineMap.h"
void CEngineWorldMap::SetMapVisibility(long id,bool visible) {
    MapList[id]->SetMapVisibility(VisibleMapList,visible);
}
void CEngineWorldMap::SetMapVisibilityAllMaps(bool visible) {
    for(unsigned long i=0;i<MapList.size();++i) {
        CEngineMap* map=MapList[i];
        if(map) map->SetMapVisibility(VisibleMapList,visible);
    }
}
