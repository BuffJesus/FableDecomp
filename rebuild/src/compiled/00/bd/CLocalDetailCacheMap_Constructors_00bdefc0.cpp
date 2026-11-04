#include "engine/CLocalDetailCacheMap.h"
#include "engine/CEngineMap.h"

CLocalDetailCacheMap::CQuadTreeElement::CQuadTreeElement(CLocalDetailCacheMap* map,long x,long y,long width,long height)
    : Map(map),MapX((unsigned short)x),MapY((unsigned short)y),Width((unsigned short)width),Height((unsigned short)height)
{
    Children[0]=0;
    Children[1]=0;
    Children[2]=0;
    Children[3]=0;
    ObjectCacheGroupCollections=0;
    StateFlags&=0xf0;
    MaxFadeDistance=0.0f;
    BoundingSphere.Center.X=0.0f;
    BoundingSphere.Center.Y=0.0f;
    BoundingSphere.Center.Z=0.0f;
    BoundingSphere.Radius=0.0f;
    FileBlockPos=0;
    FileBlockSize=0;
    OffsetIntoFileBlock=0;
    PrimitiveTypeMask=0;
}

CLocalDetailCacheMap::CLocalDetailCacheMap(CEngineMap* map)
    : LockedFromMemoryRelocation(false),StaticMapFileBlockManager(0),QuadTreeElement(this,0,0,map->MapWidth,map->MapHeight)
{
    OwnerMap=map;
}
