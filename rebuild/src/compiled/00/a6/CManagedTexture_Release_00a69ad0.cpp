#define FABLE_MANAGED_TEXTURE_INLINE_MEMBERS
#define FABLE_INLINE_PROVIDER_ANCHOR
#include "fable_managed_texture.h"
bool CManagedTexture::Release() {
    if(Data.IsNull())return true;
    bool released=false;
    if(!Data.IsNull() && Data->IVRefCount==2) {
        Data->ReleaseTexture();
        released=true;
    }
    Data.Release();
    Data.data_=0;
    return released;
}
// Keep the original callable provider emitted while permitting inline copies.
void FableManagedTextureReleaseProviderAnchor(CManagedTexture& texture){texture.Release();}
