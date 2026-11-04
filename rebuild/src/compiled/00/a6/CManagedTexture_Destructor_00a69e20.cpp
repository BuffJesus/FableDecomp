#define FABLE_MANAGED_TEXTURE_INLINE_MEMBERS
#define FABLE_INLINE_PROVIDER_ANCHOR
#include "fable_managed_texture.h"
CManagedTexture::~CManagedTexture(){if(!Data.IsNull() && Data->IVRefCount==2)Release();}
void FableManagedTextureDestructorProviderAnchor(CManagedTexture& texture){texture.CManagedTexture::~CManagedTexture();}
