#include <stdio.h>
#include "candidates/mouse_dx_accessors_compiled.h"

void CMouseDX::SetPos(const C2DVector*) {}

int main()
{
    CMouseDX mouse;
    C2DVector output;
    mouse.PositionX = 12.25f;
    mouse.PositionY = -6.5f;
    output.X = output.Y = 0.0f;
    mouse.GetPos(&output);
    if (output.X != mouse.PositionX || output.Y != mouse.PositionY)
        return 1;
    printf("MOUSE_DX_GETPOS_TEST PASS\n");
    return 0;
}
