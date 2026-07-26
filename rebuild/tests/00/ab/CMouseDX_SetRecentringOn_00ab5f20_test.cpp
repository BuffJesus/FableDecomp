#include <stdio.h>
#include "candidates/mouse_dx_accessors_compiled.h"

int main()
{
    fable_u8 storage[0x3440];
    CMouseDX* mouse = reinterpret_cast<CMouseDX*>(storage);
    mouse->RecentringOn = 0;
    mouse->CMouseDX::SetRecentringOn(true);
    if (mouse->RecentringOn != 1) return 1;
    mouse->CMouseDX::SetRecentringOn(false);
    if (mouse->RecentringOn != 0) return 2;
    printf("MOUSE_DX_SETRECENTRINGON_TEST PASS\n");
    return 0;
}
