#include "engine/CTextureManager.h"
#include "fable_managed_texture.h"
bool CTextureManager::AllocateTexture(CManagedTexture& texture,unsigned long width,unsigned long height,unsigned int levels,const CPixelFormat& format,CManagedTextureOwner* owner)
{
    long poolIndex=GetPoolIndex(CalculateTextureSize(width,height,true,format));
    if(poolIndex==-1)return false;
    CManagedTextureData* data=PreallocPools[poolIndex].Allocate();
    if(data&&data->CreateTexture(width,height,levels,format))
    {
        data->Owner=owner;
        if(texture.Data.data_){texture.Data.data_->Release();texture.Data.data_=0;}
        texture.Data.data_=data;
        data->AddRef();
        return true;
    }
    ++FailedAllocations[poolIndex];
    return false;
}
bool CTextureManager::AllocateVolumeTexture(CManagedTexture& texture,unsigned long width,unsigned long height,unsigned long depth,unsigned int levels,const CPixelFormat& format,CManagedTextureOwner* owner)
{
    long poolIndex=GetPoolIndex(CalculateTextureSize(width,height,false,format)*depth);
    if(poolIndex==-1)return false;
    CManagedTextureData* data=PreallocPools[poolIndex].Allocate();
    if(data&&data->CreateVolumeTexture(width,height,depth,levels,format))
    {
        data->Owner=owner;
        if(texture.Data.data_){texture.Data.data_->Release();texture.Data.data_=0;}
        texture.Data.data_=data;
        data->AddRef();
        return true;
    }
    ++FailedAllocations[poolIndex];
    return false;
}

