// CTexture::GenerateMipmaps(unsigned long, unsigned long) @ 0x009F9E60
// (Ego_r/FableWin ?GenerateMipmaps@CTexture@@QAEXKK@Z): D3DX filters the chain
// below the source level.
#include "fable_surface_types.h"

// d3dx9tex.h (d3dx9_25 import thunk 0x00BFEFCE, IAT 0x014405EC).
extern "C" HRESULT __stdcall D3DXFilterTexture(IDirect3DBaseTexture9* pBaseTexture,
    const PALETTEENTRY* pPalette, UINT SrcLevel, DWORD Filter);

void CTexture::GenerateMipmaps(unsigned long filter, unsigned long sourceLevel)
{
    D3DXFilterTexture(PD3DTexture, 0, sourceLevel, filter);
}
