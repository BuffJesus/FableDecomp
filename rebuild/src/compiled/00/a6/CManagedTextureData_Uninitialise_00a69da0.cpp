#include "fable_managed_texture_data.h"
#include "fable_surface_types.h"
void CManagedTextureData::Uninitialise(){
    if(Texture.Info){
        --Texture.Info->RefCount;
        if(Texture.Info->RefCount==0){
            Texture.Info->DeleteFunc(Texture.Info->Data);
            delete Texture.Info;
        }
    }
    Texture.Info=0;
    Texture.Data=0;
    D3DHeader=0;
    Data=0;
    AllocSize=0;
    // The original PC body clears this handle before the tail call.
    Texture->Uninitialise();
}
