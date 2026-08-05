#include <stdio.h>
#include <string.h>

struct CScriptedMapBrush;
extern "C" unsigned long CScriptedMapBrush_GetTheme();

extern "C" __declspec(naked) unsigned long CallGetTheme(
    const CScriptedMapBrush*, long, long, unsigned long)
{
    __asm {
        mov ecx, dword ptr [esp+4]
        mov eax, dword ptr [esp+8]
        mov edx, dword ptr [esp+12]
        push dword ptr [esp+16]
        push edx
        push eax
        call CScriptedMapBrush_GetTheme
        ret
    }
}

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
    unsigned long inside = CallGetTheme(brush, 11, 21, 2);
    unsigned long low = CallGetTheme(brush, 9, 21, 2);
    unsigned long high = CallGetTheme(brush, 13, 21, 2);
    if (inside != 0xA1B2C3D4UL) return 1;
    if (low != 0) return 2;
    if (high != 0) return 3;
    printf("SCRIPTED_BRUSH_GET_THEME_TEST PASS\n");
    return 0;
}
