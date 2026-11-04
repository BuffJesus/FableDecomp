#include "engine/CEngineWorldMap.h"
#include "engine/CIDrawWorldMap.h"
#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CEngineLocalDetailGenerator.h"
#include "engine/CLocalDetailCacheMap.h"
#include "candidates/landscape_background_lod_compiled.h"
#include "fable_engine_internal.h"
#include "fable_engine_map_list.h"

void CEngineWorldMap::AddMap(long id,long flags)
{
    if(Map) {
        delete Map;
        Map=0;
    }
    if((unsigned long)id>=MapList.size()) MapList.resize(id+1,static_cast<CEngineMap*>(0));
    CDrawMapInfo info=GameWorldMap->DrawGetMapInfo(id);
    if(!MapList[id] || MapList[id]->MapUID!=(unsigned long)info.UID) {
        DeleteMap(id);
        MapList[id]=new CEngineMap(info.PMap,info.Pos.X,info.Pos.Y,info.Extents.X,info.Extents.Y,info.UID,info.StaticMeshName,false);
        NEngineInternal::GPLandscapeRenderer->CreateMap(MapList[id],flags);
        if(!(flags&1)) NEngineInternal::GPLocalDetailGenerator->CreateMap(MapList[id]);
        CEngineMap* map=MapList[id];
        if(!map->IsInVisibleMapList) {
            VisibleMapList.push_front(map);
            map->VisibleMapListIterator=VisibleMapList.begin();
            map->IsInVisibleMapList=true;
        }
        if(map->LandscapeMap) map->LandscapeMap->OnMapVisibilityChanged(true);
        if(map->LocalDetailCacheMap) map->LocalDetailCacheMap->OnMapVisibilityChanged(true);
    }
}
