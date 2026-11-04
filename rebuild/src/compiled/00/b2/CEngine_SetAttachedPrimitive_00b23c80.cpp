#include "fable_engine.h"
#include "fable_engine_internal.h"
#include "engine/CEnginePrimitiveAttachmentManager.h"
void CEngine::SetAttachedPrimitive(CEnginePrimitiveHandle& handle,const CEnginePrimitive& primitive,const CEnginePrimitiveAttachmentInfo& info,const CEnginePrimitiveHandle& parent,long boneIndex,unsigned long flags)
{
    NEngineInternal::GPPrimitiveAttachmentManager->SetAttachedPrimitive(handle,primitive,info,parent,boneIndex,flags);
}
