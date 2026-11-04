// CTexture::GenerateMipmaps (0x009F9E60): D3DXFilterTexture(texture, no palette, sourceLevel, filter).
// The test supplies D3DXFilterTexture and records its arguments.
#include "fable_surface_types.h"
#include <stdio.h>
#include <string.h>

static void* seenTexture;
static const void* seenPalette = (const void*)1;
static unsigned int seenLevel;
static unsigned long seenFilter;

extern "C" HRESULT __stdcall D3DXFilterTexture(IDirect3DBaseTexture9* texture,
    const PALETTEENTRY* palette, UINT sourceLevel, DWORD filter)
{
    seenTexture = texture; seenPalette = palette; seenLevel = sourceLevel; seenFilter = filter;
    return 0;
}

int main()
{
    union { double align; unsigned char bytes[sizeof(CTexture)]; } storage;
    memset(&storage, 0, sizeof(storage));
    CTexture* texture = reinterpret_cast<CTexture*>(storage.bytes);
    texture->PD3DTexture = reinterpret_cast<IDirect3DTexture9*>(0xDEAD);
    texture->GenerateMipmaps(11, 22);
    if (seenTexture == (void*)0xDEAD && seenPalette == 0 && seenLevel == 22 && seenFilter == 11)
        printf("009f9e60_TEST PASS\n");
    else
        printf("FAIL p=%p pal=%p level=%u filter=%lu\n", seenTexture, seenPalette, seenLevel, seenFilter);
    return 0;
}
