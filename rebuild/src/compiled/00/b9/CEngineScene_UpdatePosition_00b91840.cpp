#include "engine/CEngineScene.h"
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEnginePrimitiveTypeInfo.h"
#include "fable_engine_primitive_manager.h"
#include "fable_engine_internal.h"

void FABLE_FASTCALL CEngineScene::UpdatePosition(CEngineInternalPrimitiveBase& primitive,unsigned long flags)
{
    if(!(flags&0x40)) {
        primitive.RemoveFromList();
        NEngineInternal::GPPrimitiveTypeInfo->PrimitiveManagers[primitive.Type]->AddPrimitive(primitive,flags);
    }
}

