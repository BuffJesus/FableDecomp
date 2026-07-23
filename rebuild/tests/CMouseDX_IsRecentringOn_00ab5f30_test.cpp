#include <stdio.h>
#include "candidates/mouse_dx_accessors_compiled.h"

int main()
{
    fable_u8 storage[0x3440];
    CMouseDX* mouse = reinterpret_cast<CMouseDX*>(storage);
    mouse->RecentringOn = 0;
    if (mouse->CMouseDX::IsRecentringOn()) return 1;
    mouse->RecentringOn = 1;
    if (!mouse->CMouseDX::IsRecentringOn()) return 2;
    printf("MOUSE_DX_ISRECENTRINGON_TEST PASS\n");
    return 0;
}
