#include <stdio.h>
#include "candidates/mouse_dx_accessors_compiled.h"

int main()
{
    fable_u8 storage[0x4850];
    CMouseDX* mouse = reinterpret_cast<CMouseDX*>(storage);
    mouse->SetDrawCursor(true);
    if (!mouse->DrawCursor) return 1;
    mouse->SetDrawCursor(false);
    if (mouse->DrawCursor) return 2;
    printf("MOUSE_DX_SETDRAWCURSOR_TEST PASS\n");
    return 0;
}
