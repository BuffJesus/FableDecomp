#include "fable_game_event.h"
#include <stdio.h>
static unsigned long storage[(sizeof(CGameEventPackage) + 3) / 4];
int main()
{
    const CGameEventPackage* package = reinterpret_cast<const CGameEventPackage*>(storage);
    const unsigned long indices[] = {0, 1, 17, 39};
    for (unsigned int i = 0; i < 4; ++i)
        if (package->GetEvent(indices[i]) != reinterpret_cast<const CGameEvent*>(
            reinterpret_cast<const unsigned char*>(package) + 8 + indices[i] * 0x28)) return 1;
    printf("EVENT_PACKAGE_GET_EVENT_PASS cases=4\n");
    return 0;
}
