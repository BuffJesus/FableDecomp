// Fixture: CPixelFormat::Initialise(D3DFORMAT) (0x009E3830) and
// CPixelFormat::FindFormatInfoIndexFromD3DFormat (0x009E3620), same TU.
#include <stdio.h>
#include "fable_surface.h"
#include "fable_d3d_format_info.h"

static int g_fail;
#define EXPECT(c) do { if (!(c)) { printf("FAIL %d: %s\n", __LINE__, #c); ++g_fail; } } while (0)

extern const CD3DFormatInfo GD3DFormats[] = {
    {(D3DFORMAT)20, PF_TYPE_RGBA, 24, 0, 8, 8, 8, 0, false},
    {(D3DFORMAT)21, PF_TYPE_RGBA, 32, 8, 8, 8, 8, 0, false},
    {D3DFMT_DXT1, PF_TYPE_COMPRESSED_BOOLEAN_ALPHA, 4, 0, 0, 0, 0, 0, false},
    {(D3DFORMAT)21, PF_TYPE_RGBA, 32, 8, 8, 8, 8, 0, false},   // duplicate: first wins
    {(D3DFORMAT)99, PF_TYPE_UNKNOWN, -1, 0, 0, 0, 0, 0, false},
    {(D3DFORMAT)77, PF_TYPE_DEPTH, 32, 0, 0, 0, 0, 0, false},   // past the terminator
};

int main()
{
    CPixelFormat f[3];
    f[0].FormatIndex = 111; f[1].FormatIndex = 222; f[2].FormatIndex = 333;
    f[1].Initialise((D3DFORMAT)20); EXPECT(f[1].FormatIndex == 0);
    f[1].Initialise((D3DFORMAT)21); EXPECT(f[1].FormatIndex == 1);
    f[1].Initialise(D3DFMT_DXT1); EXPECT(f[1].FormatIndex == 2);
    f[1].Initialise((D3DFORMAT)77); EXPECT(f[1].FormatIndex == -1);   // not searched past BitDepth -1
    f[1].Initialise((D3DFORMAT)99); EXPECT(f[1].FormatIndex == -1);   // terminator entry itself is not matched
    EXPECT(f[0].FormatIndex == 111 && f[2].FormatIndex == 333);
    EXPECT(CPixelFormat::FindFormatInfoIndexFromD3DFormat((D3DFORMAT)21) == 1);
    EXPECT(CPixelFormat::FindFormatInfoIndexFromD3DFormat(D3DFMT_DXT1) == 2);
    EXPECT(CPixelFormat::FindFormatInfoIndexFromD3DFormat((D3DFORMAT)77) == -1);
    EXPECT(CPixelFormat::FindFormatInfoIndexFromD3DFormat((D3DFORMAT)99) == -1);
    if (g_fail) { printf("FAILED %d\n", g_fail); return 1; }
    printf("FABLETLC_PIXEL_FORMAT_INITIALISE PASS\n");
    return 0;
}
