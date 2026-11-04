// CPixelFormat::Initialise(D3DFORMAT) (retail 0x009E3830) and
// CPixelFormat::FindFormatInfoIndexFromD3DFormat (retail 0x009E3620): one TU, as
// retail inlines the search into Initialise (FableWin.pdb names).
#include "fable_surface.h"
#include "fable_d3d_format_info.h"

long FABLE_FASTCALL CPixelFormat::FindFormatInfoIndexFromD3DFormat(D3DFORMAT format)
{
    long index = 0;
    do
    {
        if (GD3DFormats[index].D3DFormat == format)
            return index;
        ++index;
    } while (GD3DFormats[index].BitDepth != -1);
    return -1;
}

void CPixelFormat::Initialise(D3DFORMAT format)
{
    FormatIndex = FindFormatInfoIndexFromD3DFormat(format);
}
