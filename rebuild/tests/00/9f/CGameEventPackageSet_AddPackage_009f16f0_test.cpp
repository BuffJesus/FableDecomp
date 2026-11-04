#include "fable_game.h"
#include <stdio.h>
#include <string.h>

static unsigned long storage[(sizeof(CGameEventPackageSet) + 3) / 4];
static unsigned char packageBytes[sizeof(CGameEventPackage)];

int main()
{
    CGameEventPackageSet* set = reinterpret_cast<CGameEventPackageSet*>(storage);
    for (unsigned int i = 0; i < sizeof(packageBytes); ++i)
        packageBytes[i] = static_cast<unsigned char>(i * 31 + 7);
    set->AddPackage(reinterpret_cast<const CGameEventPackage*>(packageBytes));
    if (storage[0] != 1 || memcmp(reinterpret_cast<unsigned char*>(storage) + 4,
                                   packageBytes, sizeof(packageBytes))) return 1;
    printf("PACKAGE_SET_ADD_PACKAGE_PASS\n");
    return 0;
}
