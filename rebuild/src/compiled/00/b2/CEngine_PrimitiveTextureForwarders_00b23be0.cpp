#include "fable_engine.h"
#include "fable_engine_internal.h"
#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CEngineResourceManager.h"

void CEngine::SetPrimitiveAnimation(CEnginePrimitiveHandle& handle,const CEnginePrimitiveAnimation& animation)
{
    NEngineInternal::GPPrimitiveRenderer->SetPrimitiveAnimation(handle,animation);
}
void CEngine::SetPrimitivePosition(CEnginePrimitiveHandle& handle,const CEnginePrimitivePosition& position)
{
    NEngineInternal::GPPrimitiveRenderer->SetPrimitivePosition(handle,position);
}
CTexture* CEngine::AllocateTextureFromEngineResourcePools(unsigned long flags,const C2DExtentsI& size,long levels,const CPixelFormat& format)
{
    return NEngineInternal::GPResourceManager->AllocateExternalTextureFromResourcePools(flags,size,levels,format);
}
void CEngine::FreeTextureFromEngineResourcePools(CTexture* texture)
{
    NEngineInternal::GPResourceManager->FreeExternalTextureFromResourcePools(texture);
}
