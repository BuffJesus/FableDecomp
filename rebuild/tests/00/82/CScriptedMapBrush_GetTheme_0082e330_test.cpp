#include <stdio.h>
#include <string.h>

struct CScriptedMapBrush;
unsigned long __fastcall CScriptedMapBrush_GetTheme(
    const CScriptedMapBrush* self,
    int,
    long x,
    long y,
    unsigned long layer);

struct BrushFixture
{
    unsigned char object[0x48];
    unsigned long cells[60];
};

int main()
{
    BrushFixture fixture;
    memset(&fixture, 0, sizeof(fixture));
    *reinterpret_cast<unsigned long**>(fixture.object + 0x08) = fixture.cells;
    *reinterpret_cast<long*>(fixture.object + 0x38) = 10;
    *reinterpret_cast<long*>(fixture.object + 0x3C) = 20;
    *reinterpret_cast<long*>(fixture.object + 0x40) = 13;
    *reinterpret_cast<long*>(fixture.object + 0x44) = 22;
    fixture.cells[1 + 2 + (((21 - 20) * 3 - 10 + 11) * 5)] = 0xA1B2C3D4UL;
    const CScriptedMapBrush* brush =
        reinterpret_cast<const CScriptedMapBrush*>(fixture.object);
    if (CScriptedMapBrush_GetTheme(brush, 0, 11, 21, 2) != 0xA1B2C3D4UL) return 1;
    if (CScriptedMapBrush_GetTheme(brush, 0, 9, 21, 2) != 0) return 2;
    if (CScriptedMapBrush_GetTheme(brush, 0, 13, 21, 2) != 0) return 3;
    printf("SCRIPTED_BRUSH_GET_THEME_TEST PASS\n");
    return 0;
}
