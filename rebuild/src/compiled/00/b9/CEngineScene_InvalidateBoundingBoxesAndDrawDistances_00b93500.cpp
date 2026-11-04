#include "engine/CEngineScene.h"
#include "engine/CEngineSceneMap.h"
#include "engine/CEngineSceneGrid.h"

void CEngineScene::InvalidateBoundingBoxesAndDrawDistances()
{
    for(SceneMaps::iterator column=MapList.begin();column!=MapList.end();++column)
        for(MapColumn::iterator map=column->second.begin();map!=column->second.end();++map)
            map->second.Data->GridLevels[0].Data->InvalidateBoundingBoxAndDrawDistance(0,0,true);
}
