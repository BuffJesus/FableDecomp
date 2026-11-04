#include "fable_engine.h"
#include "fable_engine_internal.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CEngineSurfaceCompositionManager.h"
unsigned long CEngine::AddCustomMorphTexture(CTexture& texture,const CCharString& name) const
{
    return NEngineInternal::GPSurfaceCompositionManager->AddCustomMorphTexture(texture,name);
}
void CEngine::UpdateTextureMorphCache(const CArray<CEngineTextureMorph>& morphs,bool force,float time,const CIEngineLongSet* used)
{
    NEngineInternal::GPResourceManager->GraphicBanks->ChangeScene();
    NEngineInternal::GPSurfaceCompositionManager->UpdateDiskCache(morphs,force,time,used);
}
