#include "engine/CEngineWorldMap.h"
#include "fable_stlport_vector_impl.h"
void CEngineWorldMap::DeleteMap() {
    for(unsigned i=0;i<MapList.size();++i) DeleteMap(i);
    MapList.resize(0);
    if(Map) {
        delete Map;
        Map=0;
    }
}
