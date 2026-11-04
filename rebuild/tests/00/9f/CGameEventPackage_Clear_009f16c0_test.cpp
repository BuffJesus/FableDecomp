#include "fable_game_event.h"
#include <stdio.h>
#include <string.h>
static unsigned long storage[(sizeof(CGameEventPackage) + 3) / 4];
int main()
{
    memset(storage, 0xa5, sizeof(storage));
    CGameEventPackage* package = reinterpret_cast<CGameEventPackage*>(storage);
    package->Clear();
    if (storage[1] != 0 || storage[0] != 0xa5a5a5a5UL || storage[2] != 0xa5a5a5a5UL) return 1;
    printf("EVENT_PACKAGE_CLEAR_PASS\n");
    return 0;
}
