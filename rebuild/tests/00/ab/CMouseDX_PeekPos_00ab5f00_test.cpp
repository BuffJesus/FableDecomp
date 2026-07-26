#include <stdio.h>
#include "candidates/mouse_dx_accessors_compiled.h"

int main()
{
    fable_u8 storage[0x3440];
    CMouseDX* mouse = reinterpret_cast<CMouseDX*>(storage);
    mouse->PositionX = 7.25f;
    mouse->PositionY = -4.0f;
    const C2DVector& position = mouse->CMouseDX::PeekPos();
    if (&position != reinterpret_cast<C2DVector*>(&mouse->PositionX)) return 1;
    if (position.X != 7.25f || position.Y != -4.0f) return 2;
    printf("MOUSE_DX_PEEKPOS_TEST PASS\n");
    return 0;
}
