#include <stdio.h>
#include <string.h>
#include "candidates/scripted_map_brush_theme.h"

struct BrushFixture
{
    unsigned char object[0x48];
    unsigned char cells[240];
};

int main()
{
    BrushFixture fixture;
    memset(&fixture, 0, sizeof(fixture));
    *reinterpret_cast<unsigned char**>(fixture.object + 0x08) = fixture.cells;
    *reinterpret_cast<long*>(fixture.object + 0x38) = 10;
    *reinterpret_cast<long*>(fixture.object + 0x3C) = 20;
    *reinterpret_cast<long*>(fixture.object + 0x40) = 13;
    *reinterpret_cast<long*>(fixture.object + 0x44) = 22;
    CScriptedMapBrush* brush = reinterpret_cast<CScriptedMapBrush*>(fixture.object);
    brush->SetThemeBlend(11, 21, 0xB6, 2);
    if (fixture.cells[((21 - 20) * 3 - 10 + 11) * 0x14 + 0x10 + 2] != 0xB6) return 1;
    brush->SetThemeBlend(13, 21, 0xCC, 2);
    if (fixture.cells[((21 - 20) * 3 - 10 + 11) * 0x14 + 0x10 + 2] != 0xB6) return 2;
    printf("SCRIPTED_BRUSH_SET_BLEND_TEST PASS\n");
    return 0;
}
