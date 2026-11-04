#pragma once

// CGraphicDataBankInit: Ego_r.pdb layout (0x2C), matching retail's stack
// object in CNewFrontendGameComponent::InitialiseGraphics 0x0042DDB3.

#include <stddef.h>

#include "fable_surface.h"

class CGraphicDataBankInit
{
public:
    // Retail 0x00415B80 (out of line).
    CGraphicDataBankInit();

    CPixelFormat NonAlphaFormat;                // +0x00
    CPixelFormat AlphaFormat;                   // +0x04
    CPixelFormat InterpolatedAlphaFormat;       // +0x08
    CPixelFormat BooleanAlphaFormat;            // +0x0C
    CPixelFormat UncompressedNonAlphaFormat;    // +0x10
    CPixelFormat UncompressedAlphaFormat;       // +0x14
    CPixelFormat SignedFormat;                  // +0x18
    bool GenerateMipmaps;                       // +0x1C
    unsigned long MaxGraphicWidth;              // +0x20
    unsigned long MaxGraphicHeight;             // +0x24
    bool AllowDither;                           // +0x28
    bool AllowCompressedTextures;               // +0x29
    bool AllowDXT1ForBooleanAlpha;              // +0x2A
};

FABLE_STATIC_ASSERT(offsetof(CGraphicDataBankInit, SignedFormat) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CGraphicDataBankInit, MaxGraphicWidth) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CGraphicDataBankInit, AllowDither) == 0x28);
FABLE_STATIC_ASSERT(sizeof(CGraphicDataBankInit) == 0x2C);
