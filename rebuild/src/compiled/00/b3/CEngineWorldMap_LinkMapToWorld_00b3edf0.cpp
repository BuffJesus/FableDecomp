#include "engine/CEngineWorldMap.h"
#include "engine/CEngineMap.h"
void CEngineWorldMap::LinkMapToWorld(CEngineMap* map,long id) {
    long x0=(map->WorldPosX-MapOffsetX)>>5;
    long y0=(map->WorldPosY-MapOffsetY)>>5;
    long x1=(map->WorldPosX+map->MapWidth-MapOffsetX)>>5;
    long y1=(map->WorldPosY+map->MapHeight-MapOffsetY)>>5;
    for(long y=y0;y<y1;++y)
        for(long x=x0;x<x1;++x)
            Map[y*MapArrayWidth+x]=(short)id;
}
