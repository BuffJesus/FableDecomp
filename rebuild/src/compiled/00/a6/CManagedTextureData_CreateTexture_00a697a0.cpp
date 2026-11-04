#include "fable_managed_texture_data.h"
#include "fable_surface_types.h"
bool CManagedTextureData::CreateTexture(unsigned long width,unsigned long height,unsigned int levels,const CPixelFormat& format)
{
    C2DExtentsI dimensions=CTexture::GetNextValidDimensions(C2DExtentsI(width,height),format);
    Width=(unsigned short)dimensions.X;
    Height=(unsigned short)dimensions.Y;
    Allocated=Texture->Initialise(C2DExtentsI(dimensions.X,dimensions.Y),-1,format,0,SURFACE_POOL_MANAGED,false,false);
    return Allocated;
}

