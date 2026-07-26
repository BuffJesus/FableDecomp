#include <stdio.h>
#include "candidates/mouse_dx_accessors_compiled.h"

int main()
{
    fable_u8 storage[0x3440];
    CMouseDX* mouse = reinterpret_cast<CMouseDX*>(storage);
    mouse->InputOn = 0;
    mouse->CMouseDX::SetInputOn(true);
    if (mouse->InputOn != 1) return 1;
    mouse->CMouseDX::SetInputOn(false);
    if (mouse->InputOn != 0) return 2;
    printf("MOUSE_DX_SETINPUTON_TEST PASS\n");
    return 0;
}
