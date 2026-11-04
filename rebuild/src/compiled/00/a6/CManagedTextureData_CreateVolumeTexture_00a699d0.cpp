#define FABLE_VOLUME_TEXTURE_INLINE_DEFAULT_CONSTRUCTOR
#include "fable_managed_texture_data.h"
#include "fable_surface_types.h"
#include "fable_volume_types.h"

bool CManagedTextureData::CreateVolumeTexture(unsigned long width,unsigned long height,unsigned long depth,unsigned int levels,const CPixelFormat& format)
{
    C2DExtentsI dimensions=CTexture::GetNextValidDimensions(C2DExtentsI(width,height),format);
    const unsigned long validWidth=dimensions.X,validHeight=dimensions.Y;
    VolumeTexture.Reset(new CVolumeTexture);
    Width=(unsigned short)validWidth;
    Height=(unsigned short)validHeight;
    Allocated=VolumeTexture->Initialise(C3DExtentsI(validWidth,validHeight,depth),1,format,0,VOLUME_POOL_MANAGED,false);
    return Allocated;
}

