#ifndef FABLE_D3D_FORMAT_INFO_H
#define FABLE_D3D_FORMAT_INFO_H

// The engine's static pixel-format table (FableWin.pdb
// ?GD3DFormats@@3QBVCD3DFormatInfo@@B, retail 0x0129BA40). CPixelFormat is an
// index into it; CDisplayManager's GetBest*TextureFormat / IsTextureBitDepthSupported
// (0x009BE530..0x009BE7F5) and CPixelFormat::Initialise (0x009E3830) walk it
// until BitDepth == -1. Record layout: Ego_r.pdb CD3DFormatInfo (0x24 bytes).
// The retail table body is landed as rebuild/src/compiled/01/29/CPixelFormat_Table_0129ba40.cpp.

#include "fable_d3d9.h"

// FableWin.pdb enum EPixelFormatType.
enum EPixelFormatType
{
    PF_TYPE_NULL = 0,
    PF_TYPE_UNKNOWN = 1,
    PF_TYPE_RGBA = 2,
    PF_TYPE_DEPTH = 3,
    PF_TYPE_COMPRESSED_ALPHA = 4,
    PF_TYPE_COMPRESSED_BOOLEAN_ALPHA = 5,
    PF_TYPE_BUMP = 6,
    PF_TYPE_COMPRESSED_INTERPOLATED_ALPHA = 7
};

class CD3DFormatInfo
{
public:
    D3DFORMAT D3DFormat;        // +0x00
    EPixelFormatType PFType;    // +0x04
    long BitDepth;              // +0x08 (-1 terminates the table)
    long AlphaBits;             // +0x0C
    long RBits;                 // +0x10
    long GBits;                 // +0x14
    long BBits;                 // +0x18
    long StencilBits;           // +0x1C
    bool Linear;                // +0x20
};

extern const CD3DFormatInfo GD3DFormats[];

typedef char FableCD3DFormatInfoSizeMustBe24[sizeof(CD3DFormatInfo) == 0x24 ? 1 : -1];
typedef char FableCD3DFormatInfoStencilBitsAt1C[
    (unsigned long)&((CD3DFormatInfo*)0)->StencilBits == 0x1C ? 1 : -1];

#endif
