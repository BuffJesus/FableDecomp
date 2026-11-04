#include "engine/CTextureManager.h"
#include "fable_surface_types.h"
#include "fable_d3d_format_info.h"

unsigned long FABLE_FASTCALL CTextureManager::CalculateTextureSize(unsigned long width,unsigned long height,bool mipmaps,const CPixelFormat& format)
{
    unsigned long minimum=1;
    unsigned long bits;
    switch(GD3DFormats[format.FormatIndex].D3DFormat) {
    case D3DFMT_DXT1: bits=4;minimum=4;break;
    case D3DFMT_DXT3:
    case D3DFMT_DXT5: bits=8;minimum=4;break;
    default: bits=format.GetColourDepth();break;
    }
    unsigned long pixels=0;
    while(width>0 || height>0) {
        pixels+=(width>minimum?width:minimum)*(height>minimum?height:minimum);
        if(!mipmaps)break;
        width>>=1;
        height>>=1;
    }
    return (pixels*bits)>>3;
}
