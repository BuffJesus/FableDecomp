#include <new>
#include "candidates/landscape_background_lod_compiled.h"
#include "engine/CLocalDetailCacheMap.h"
#include "fable_engine_map_list.h"
void CEngineMap::SetMapVisibility(_STL::list<CEngineMap*>& list,bool visible) {
    if(visible) {
        if(!IsInVisibleMapList) {
            list.push_front(this);
            VisibleMapListIterator=list.begin();
            IsInVisibleMapList=true;
        }
    } else if(IsInVisibleMapList) {
        list.erase(VisibleMapListIterator);
        VisibleMapListIterator=list.end();
        IsInVisibleMapList=false;
    }
    if(LandscapeMap) LandscapeMap->OnMapVisibilityChanged(visible);
    if(LocalDetailCacheMap) LocalDetailCacheMap->OnMapVisibilityChanged(visible);
}
