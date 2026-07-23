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
    fixture.cells[((21 - 20) * 3 - 10 + 11) * 0x14 + 0x10 + 2] = 0xA7;
    const CScriptedMapBrush* brush =
        reinterpret_cast<const CScriptedMapBrush*>(fixture.object);
    if (brush->GetThemeBlend(11, 21, 2) != 0xA7) return 1;
    if (brush->GetThemeBlend(11, 22, 2) != 0) return 2;
    printf("SCRIPTED_BRUSH_GET_BLEND_TEST PASS\n");
    return 0;
}
