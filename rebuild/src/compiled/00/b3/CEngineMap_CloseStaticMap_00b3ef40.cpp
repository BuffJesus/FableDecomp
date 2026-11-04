#include "engine/CEngineMap.h"
#include "candidates/landscape_background_lod_compiled.h"
#include "engine/CLocalDetailCacheMap.h"
#include "fable_game_text_bank.h"
template<class T> inline void CCountedPointer<T>::Reset(T* data) {
    Release();
    Info=0;
    Data=data;
    if(Data)Info=new CCPPointerInfo(&DeleteData,Data);
}
void CEngineMap::CloseStaticMap() {
    if(StaticMapOpenState) {
        if(LandscapeMap)LandscapeMap->CloseStaticMap();
        if(LocalDetailCacheMap)LocalDetailCacheMap->CloseStaticMap();
        AsyncFileEntry.Reset(0);
        StaticMapOpenState=0;
    }
}
