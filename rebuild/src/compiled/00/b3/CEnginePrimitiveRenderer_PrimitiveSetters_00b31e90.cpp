#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEngineScene.h"

void CEnginePrimitiveRenderer::SetPrimitiveAnimation(CEnginePrimitiveHandle& handle,const CEnginePrimitiveAnimation& animation)
{
    CEngineInternalPrimitiveBase* primitive=static_cast<CEngineInternalPrimitiveBase*>(handle.Data.data_);
    if(primitive) {
        primitive->SetAnimation(animation);
        if(primitive->GridPos_Level>=0)CEngineScene::UpdatePosition(*primitive,primitive->EnginePrimitiveFlags);
    }
}

void CEnginePrimitiveRenderer::SetPrimitivePosition(CEnginePrimitiveHandle& handle,const CEnginePrimitivePosition& position)
{
    CEngineInternalPrimitiveBase* primitive=static_cast<CEngineInternalPrimitiveBase*>(handle.Data.data_);
    if(primitive) {
        primitive->SetPosition(position);
        if(primitive->GridPos_Level>=0)CEngineScene::UpdatePosition(*primitive,primitive->EnginePrimitiveFlags);
    }
}
