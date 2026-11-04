#include "engine/CEngineScene.h"
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEnginePrimitiveTypeInfo.h"
#include "fable_engine_primitive_manager.h"
#include "fable_engine_internal.h"

void CEnginePrimitiveManager::AddPrimitive(CEngineInternalPrimitiveBase& primitive,unsigned long flags)
{
    SceneList[0]->AddPrimitive(primitive,flags);
}
