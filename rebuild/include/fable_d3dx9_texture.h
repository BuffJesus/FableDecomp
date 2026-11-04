#pragma once
#include "fable_d3d9.h"
// Original d3dx9_25 import: thunk00BFEFF2 / IAT014405D4.
extern "C" HRESULT __stdcall D3DXCreateVolumeTexture(IDirect3DDevice9* device,
 unsigned int width, unsigned int height, unsigned int depth, unsigned int levels,
 unsigned long usage, D3DFORMAT format, D3DPOOL pool, IDirect3DVolumeTexture9** texture);
typedef struct tagPALETTEENTRY PALETTEENTRY;
// Original d3dx9_25 import thunk00BFEFEC.
extern "C" HRESULT __stdcall D3DXLoadVolumeFromVolume(IDirect3DVolume9* destination,
 const PALETTEENTRY* destinationPalette, const D3DBOX* destinationBox, IDirect3DVolume9* source,
 const PALETTEENTRY* sourcePalette, const D3DBOX* sourceBox, unsigned long filter, D3DCOLOR colourKey);
