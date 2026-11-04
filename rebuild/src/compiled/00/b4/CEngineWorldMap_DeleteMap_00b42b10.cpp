#include "engine/CEngineWorldMap.h"
#include "engine/CEngineMap.h"
#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CEngineLocalDetailGenerator.h"
#include "engine/CLocalDetailCacheMap.h"
#include "candidates/landscape_background_lod_compiled.h"
#include "fable_engine_internal.h"
#include "fable_stlport_vector_impl.h"
namespace _STL {
template<> inline list<CEngineMap*>::iterator list<CEngineMap*>::erase(iterator position) {
    iterator next(position);++next;
    position._M_node->_M_unhook();free(position._M_node);
    return next;
}
}
void CEngineWorldMap::DeleteMap(long id) {
    if(MapList[id]) {
        CEngineMap* map=MapList[id];
        if(map->LandscapeMap)map->LandscapeMap->OnGameMapUnloaded();
        map->GameMap=0;
        map=MapList[id];
        if(map->IsInVisibleMapList) {
            VisibleMapList.erase(map->VisibleMapListIterator);
            map->VisibleMapListIterator=VisibleMapList.end();
            map->IsInVisibleMapList=false;
        }
        if(map->LandscapeMap)map->LandscapeMap->OnMapVisibilityChanged(false);
        if(map->LocalDetailCacheMap)map->LocalDetailCacheMap->OnMapVisibilityChanged(false);
        if(Map)UnlinkMapFromWorld(MapList[id],id);
        if(NEngineInternal::GPLandscapeRenderer)NEngineInternal::GPLandscapeRenderer->DeleteMap(MapList[id]);
        if(NEngineInternal::GPLocalDetailGenerator)NEngineInternal::GPLocalDetailGenerator->DeleteMap(MapList[id]);
        delete MapList[id];
        MapList[id]=0;
        while(MapList.size() && !MapList[MapList.size()-1])MapList.resize(MapList.size()-1);
    }
}
