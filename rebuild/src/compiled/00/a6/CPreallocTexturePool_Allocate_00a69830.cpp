
#include "fable_surface_types.h"
#include "fable_managed_texture_data.h"
#include "engine/CManagedTextureOwner.h"
#include "engine/CTextureManager.h"
CManagedTextureData* CPreallocTexturePool::Allocate()
{
    for(unsigned long i=0;i<Count;++i)
    {
        CManagedTextureData* texture=static_cast<CManagedTextureData*>(Head.NextResource);
        if(!texture || texture==static_cast<CResource*>(&Head))continue;
        texture->SetAsMRU();
        if(texture->BusyFlag)texture->BusyFlag=texture->Texture->IsBusy();
        if(texture->BusyFlag&&!CTextureManager::GetReuseBusyTextures())continue;
        if(texture->Locked)continue;
        texture->Locked=true;
        if(texture->Allocated)texture->Owner->ReleaseTexture();
        texture->Locked=false;
        if(!texture->Allocated)return texture;
    }
    return 0;
}
