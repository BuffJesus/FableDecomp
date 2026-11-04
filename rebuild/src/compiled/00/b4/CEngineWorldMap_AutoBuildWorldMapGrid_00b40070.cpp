#include "engine/CEngineWorldMap.h"
#include "engine/CEngineMap.h"
#include <limits.h>
#include <string.h>
#include <new>
void CEngineWorldMap::AutoBuildWorldMapGrid() {
    if(!Map) {
        long minX=LONG_MAX,minY=LONG_MAX,maxX=0,maxY=0;
        bool haveMap=false;
        for(unsigned i=0;i<MapList.size();++i) {
            CEngineMap* map=MapList[i];
            if(map) {
                if(minX>map->WorldPosX)minX=map->WorldPosX;
                if(minY>map->WorldPosY)minY=map->WorldPosY;
                if(maxX<map->WorldPosX+map->MapWidth)maxX=map->WorldPosX+map->MapWidth;
                if(maxY<map->WorldPosY+map->MapHeight)maxY=map->WorldPosY+map->MapHeight;
                haveMap=true;
            }
        }
        if(haveMap) {
            long gridX=minX/32-1,gridY=minY/32-1;
            MapOffsetX=gridX*32;MapOffsetY=gridY*32;
            MapArrayWidth=(maxX+31)/32-gridX+1;
            MapArrayHeight=(maxY+31)/32-gridY+1;
            Map=new short[MapArrayWidth*MapArrayHeight];
            memset(Map,0xff,MapArrayWidth*MapArrayHeight*sizeof(short));
            for(unsigned i=0;i<MapList.size();++i)
                if(MapList[i])LinkMapToWorld(MapList[i],i);
        }
    }
}
