#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CEngineLocalDetailGenerator.h"
#include "engine/CEngineMap.h"
#include "engine/CLocalDetailCacheMap.h"
#include "candidates/landscape_background_lod_compiled.h"
void CEngineLandscapeRenderer::DeleteMap(CEngineMap* map) {
    delete map->LandscapeMap;
    map->LandscapeMap=0;
}
void CEngineLocalDetailGenerator::DeleteMap(CEngineMap* map) {
    if(map->LocalDetailCacheMap) {
        delete map->LocalDetailCacheMap;
        map->LocalDetailCacheMap=0;
    }
}
