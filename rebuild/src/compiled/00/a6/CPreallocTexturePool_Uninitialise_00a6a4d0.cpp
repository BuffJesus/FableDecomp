#include "engine/CPreallocTexturePool.h"
#include "fable_managed_texture_data.h"
#include "fable_surface_types.h"
void CPreallocTexturePool::Uninitialise(){
    for(unsigned long i=0;i<Count;++i){
        CManagedTextureData* data=Textures[i].data_;
        RemoveResource(*data);
        if(data->Texture.Info){
            --data->Texture.Info->RefCount;
            if(data->Texture.Info->RefCount==0){
                data->Texture.Info->DeleteFunc(data->Texture.Info->Data);
                delete data->Texture.Info;
            }
        }
        data->Texture.Info=0;data->Texture.Data=0;data->D3DHeader=0;
        data->Data=0;data->AllocSize=0;
        data->Texture->Uninitialise();
    }
    Count=0;
    Textures.DeallocateMemory();
}
