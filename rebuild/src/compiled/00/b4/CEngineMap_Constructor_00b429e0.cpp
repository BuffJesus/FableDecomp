#include "engine/CEngineMap.h"

CEngineMap::CEngineMap(const CIDrawMap* gameMap,long x,long y,long width,long height,long uid,CCharString name,bool)
{
    GameMap=gameMap;
    MapWorldPosition.X=(float)x;
    MapWorldPosition.Y=(float)y;
    MapWorldPosition.Z=0.0f;
    MapUID=uid;
    WorldPosX=(unsigned short)x;
    WorldPosY=(unsigned short)y;
    MapWidth=(unsigned short)width;
    MapHeight=(unsigned short)height;
    LandscapeMap=0;
    LocalDetailCacheMap=0;
    StaticMapName=name;
    IsInVisibleMapList=false;
}
