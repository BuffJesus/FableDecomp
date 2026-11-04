// Behaviour fixture for CGameEvent::GetDataPackage (0x009F17A0).
// Links the compiled source object.
#include "fable_game_event.h"

#include <stdio.h>
#include <string.h>

int main()
{
    int fail = 0;
    CGameEvent event(9);
    memset(event.Data, 0, sizeof(event.Data));
    for (int i = 0; i < 6; ++i)
        event.Data[i] = (unsigned char)(0xA0 + i);
    event.EndPos = 6;

    CGameEventDataPackage package;
    memset(package.Data, 0xCC, sizeof(package.Data));
    package.Length = 99;
    package.StartPos = 7;
    package.EndPos = 8;
    const CGameEvent& cevent = event;
    cevent.GetDataPackage(&package);
    // Exactly EndPos bytes copied; the rest of the package payload untouched.
    if (memcmp(package.Data, event.Data, 6) != 0 || package.Data[6] != 0xCC || package.Data[31] != 0xCC) fail |= 1;
    if (package.Length != 6 || package.StartPos != 0 || package.EndPos != 0) fail |= 2;
    // The event itself is not modified.
    if (event.EndPos != 6 || event.Type != 9 || event.Data[0] != 0xA0) fail |= 4;

    // Empty event: nothing copied, cursor reset.
    CGameEvent empty(1);
    memset(package.Data, 0xCC, sizeof(package.Data));
    package.Length = 5;
    package.StartPos = 3;
    empty.GetDataPackage(&package);
    if (package.Length != 0 || package.StartPos != 0 || package.EndPos != 0 || package.Data[0] != 0xCC) fail |= 8;

    // Full payload.
    CGameEvent full(2);
    for (int j = 0; j < 32; ++j)
        full.Data[j] = (unsigned char)j;
    full.EndPos = 32;
    full.GetDataPackage(&package);
    if (package.Length != 32 || memcmp(package.Data, full.Data, 32) != 0) fail |= 16;

    if (fail == 0)
        printf("CGAMEEVENT_GETDATAPACKAGE_PASS\n");
    else
        printf("CGAMEEVENT_GETDATAPACKAGE_FAIL %d\n", fail);
    return 0;
}