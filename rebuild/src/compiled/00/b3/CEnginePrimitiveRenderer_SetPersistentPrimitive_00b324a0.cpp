#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEnginePrimitiveTypeInfo.h"
#include "engine/CEnginePrimitiveRenderer2D.h"
#include "engine/CEngineScene.h"
#include "fable_engine_internal.h"
#include "fable_engine_primitive_manager.h"
void CEnginePrimitiveRenderer::SetPersistentPrimitive(CEnginePrimitiveHandle& handle,const CEnginePrimitive& value,unsigned long flags,bool nonpersistent)
{
    EEnginePrimitiveType type=value.Type;
    CEnginePrimitiveManager* manager=NEngineInternal::GPPrimitiveTypeInfo->PrimitiveManagers[type];
    bool enabled=(flags&0x20)==0;
    CEngineInternalPrimitiveBase* primitive=static_cast<CEngineInternalPrimitiveBase*>(handle.Data.data_);
    if(primitive) {
        if(primitive->Type==type) {
            primitive->Update(value,flags);
            primitive->Enable=enabled;
            if(primitive->GridPos_Level>=0)CEngineScene::UpdatePosition(*primitive,flags);
            return;
        }
        handle.Clear();
    }
    if(manager) {
        primitive=manager->CreateInternalPrimitive(type);
        handle.SetData(primitive);
        primitive->NonPersistent=nonpersistent;
        primitive->Update(value,flags);
        primitive->Enable=enabled;
        if(flags&0x40)NEngineInternal::GPPrimitiveRenderer2D->AddPrimitive(*primitive);
        else manager->AddPrimitive(*primitive,flags);
    }
}
