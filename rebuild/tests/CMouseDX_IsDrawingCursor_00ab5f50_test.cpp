#include <stdio.h>
#include "candidates/mouse_dx_accessors_compiled.h"

int main()
{
    fable_u8 storage[0x4850];
    CMouseDX* mouse = reinterpret_cast<CMouseDX*>(storage);
    mouse->DrawCursor = false;
    if (mouse->IsDrawingCursor()) return 1;
    mouse->DrawCursor = true;
    if (!mouse->IsDrawingCursor()) return 2;
    printf("MOUSE_DX_ISDRAWINGCURSOR_TEST PASS\n");
    return 0;
}
