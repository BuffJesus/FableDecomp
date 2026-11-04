#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CEngineScene.h"
#include "fable_engine_primitive_manager.h"
void CEnginePrimitiveRenderer::FrameEnd(const CEngineComponentParams&)
{
    for(CEngineScene** it=SceneList.begin();it!=SceneList.end();++it)(*it)->FrameEnd();
    for(unsigned long i=0;i<PrimitiveManagers.size();++i)PrimitiveManagers[i].Data->FrameEnd();
    for(unsigned long i=0;i<SceneList.size();++i)SceneList[i]->ResetStats();
    DrawDistanceInvalid=false;
}
