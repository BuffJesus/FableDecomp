// Fixture: CPixelFormat::GetColourDepth (0x009E3820).
#include <stdio.h>
#include "fable_surface.h"
#include "fable_d3d_format_info.h"

static int g_fail;
#define EXPECT(c) do { if (!(c)) { printf("FAIL %d: %s\n", __LINE__, #c); ++g_fail; } } while (0)

extern const CD3DFormatInfo GD3DFormats[] = {
    {(D3DFORMAT)20, PF_TYPE_RGBA, 24, 0, 8, 8, 8, 0, false},
    {(D3DFORMAT)23, PF_TYPE_RGBA, 16, 0, 5, 6, 5, 0, false},
    {D3DFMT_DXT1, PF_TYPE_COMPRESSED_BOOLEAN_ALPHA, 4, 0, 0, 0, 0, 0, false},
    {(D3DFORMAT)99, PF_TYPE_UNKNOWN, -1, 0, 0, 0, 0, 0, false},
};

int main()
{
    CPixelFormat f;
    f.FormatIndex = 0; EXPECT(f.GetColourDepth() == 24);
    f.FormatIndex = 1; EXPECT(f.GetColourDepth() == 16);
    f.FormatIndex = 2; EXPECT(f.GetColourDepth() == 4);
    EXPECT(f.FormatIndex == 2);
    if (g_fail) { printf("FAILED %d\n", g_fail); return 1; }
    printf("FABLETLC_PIXEL_FORMAT_COLOUR_DEPTH PASS\n");
    return 0;
}
