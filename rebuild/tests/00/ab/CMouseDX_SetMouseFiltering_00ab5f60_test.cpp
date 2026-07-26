#include <stdio.h>
#include "candidates/mouse_dx_accessors_compiled.h"

int main()
{
    fable_u8 storage[0x4850];
    CMouseDX* mouse = reinterpret_cast<CMouseDX*>(storage);
    mouse->SetMouseFiltering(true);
    if (!mouse->MouseFiltering) return 1;
    mouse->SetMouseFiltering(false);
    if (mouse->MouseFiltering) return 2;
    printf("MOUSE_DX_SETMOUSEFILTERING_TEST PASS\n");
    return 0;
}
