#include "engine/CEngineScene.h"
#include "engine/CEngineSceneMap.h"
void CEngineScene::UpdateMaps()
{
    for(SceneMaps::iterator column=MapList.begin();column!=MapList.end();++column)
        for(MapColumn::iterator map=column->second.begin();map!=column->second.end();++map)
            map->second.Data->Update();
}
