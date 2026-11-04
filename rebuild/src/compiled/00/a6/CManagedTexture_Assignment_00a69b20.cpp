#define FABLE_MANAGED_TEXTURE_INLINE_MEMBERS
#define FABLE_INLINE_PROVIDER_ANCHOR
#include "fable_managed_texture.h"
const CManagedTexture& CManagedTexture::operator=(const CManagedTexture& other){
    if(Data.data_!=other.Data.data_){Release();Data=other.Data;}
    return *this;
}
void FableManagedTextureAssignmentProviderAnchor(CManagedTexture& left,const CManagedTexture& right){left=right;}
