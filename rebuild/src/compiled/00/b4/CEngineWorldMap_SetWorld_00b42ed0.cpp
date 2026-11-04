#include "engine/CEngineWorldMap.h"
#include "engine/CEngineMap.h"
#include "engine/CIDrawWorldMap.h"
#include "fable_world.h"
#include "fable_engine_internal.h"
#include "candidates/landscape_background_lod_compiled.h"
void CEngineWorldMap::SetWorld(const CIDrawWorld* world) {
    if (world != GameWorld) {
        GameWorld=world;
        GameWorldMap=const_cast<CIDrawWorldMap*>(world->DrawGetWorldMap());
        DeleteMap();
        long count=GameWorldMap->DrawGetNoMaps();
        MapList.resize(count,static_cast<CEngineMap*>(0));
        for(long i=1;i<count;++i) {
            CDrawMapInfo info=GameWorldMap->DrawGetMapInfo(i);
            if(info.Valid) AddMap(i,0);
        }
        for(long i=1;i<count;++i) {
            CDrawMapInfo info=GameWorldMap->DrawGetMapInfo(i);
            if(info.Valid && info.PMap) {
                CEngineMap* map=MapList[i];
                CDrawMapInfo loaded=NEngineInternal::GPEngineWorldMap->GameWorldMap->DrawGetMapInfo(i);
                map->GameMap=loaded.PMap;
                map->LandscapeMap->OnGameMapLoaded();
            }
        }
        AutoBuildWorldMapGrid();
    }
}
