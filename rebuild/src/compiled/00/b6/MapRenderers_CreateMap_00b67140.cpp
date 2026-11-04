#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CEngineLocalDetailGenerator.h"
#include "engine/CLocalDetailCacheMap.h"
#include "candidates/landscape_background_lod_compiled.h"

FABLE_STATIC_ASSERT(sizeof(CEngineLandscapeMap)==0x60);
FABLE_STATIC_ASSERT(sizeof(CLocalDetailCacheMap)==0x70);

void CEngineLandscapeRenderer::CreateMap(CEngineMap* map,long flags)
{
    map->LandscapeMap=new CEngineLandscapeMap(map,flags);
}

void CEngineLocalDetailGenerator::CreateMap(CEngineMap* map)
{
    if(!map->LocalDetailCacheMap) map->LocalDetailCacheMap=new CLocalDetailCacheMap(map);
}
