#include "fable_game.h"
#include <stdio.h>
#include <string.h>

static unsigned long storage[(sizeof(CGameEventPackageSet) + 3) / 4];

int main()
{
    CGameEventPackageSet* set = reinterpret_cast<CGameEventPackageSet*>(storage);
    memset(storage, 0xa5, sizeof(storage));
    set->Clear();
    if (storage[0] != 0 || reinterpret_cast<unsigned char*>(storage)[4] != 0xa5) return 1;
    printf("PACKAGE_SET_CLEAR_PASS\n");
    return 0;
}
