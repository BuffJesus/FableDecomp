#include "fable_managed_texture_data.h"
#include "fable_surface_types.h"
void CManagedTextureData::Initialise(unsigned long allocationSize,unsigned long poolIndex)
{
 Texture.Reset(new CTexture);
 AllocSize=allocationSize;
 PoolIndex=poolIndex;
}
