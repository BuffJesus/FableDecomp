#pragma once
#include "fable_d3d9.h"

// D3D9's three 256-entry WORD channels. Retail SetGamma allocates 0x600 bytes
// and writes the same value at channel offsets 0, 0x200 and 0x400.
struct _D3DGAMMARAMP
{
    unsigned short red[256];
    unsigned short green[256];
    unsigned short blue[256];
};
FABLE_STATIC_ASSERT(sizeof(D3DGAMMARAMP)==0x600);
