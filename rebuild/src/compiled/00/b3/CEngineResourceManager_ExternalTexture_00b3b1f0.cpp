#include "../../../../include/engine/CEngineResourceManager.h"
#include "../../../../include/fable_surface_types.h"

CTexture* CEngineResourceManager::AllocateExternalTextureFromResourcePools(unsigned long,const C2DExtentsI& dimensions,long levels,const CPixelFormat& format)
{
    CTexture* texture=new CTexture;
    texture->Initialise(dimensions,levels,format,0,SURFACE_POOL_MANAGED,false,false);
    return texture;
}

void CEngineResourceManager::FreeExternalTextureFromResourcePools(CTexture* texture)
{
    delete texture;
}
