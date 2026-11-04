#include "fable_surface_types.h"
long CTexture::GetLevelByteLength(long level)const{D3DSURFACE_DESC description,base;PD3DTexture->GetLevelDesc(level,&description);PD3DTexture->GetLevelDesc(0,&base);CPixelFormat format(base.Format);return (format.GetColourDepth()*description.Width*description.Height)>>3;}
