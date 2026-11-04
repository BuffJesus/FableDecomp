#include "fable_game_event.h"
#include <stdio.h>
static unsigned long storage[(sizeof(CGameEventPackage) + 3) / 4];
int main()
{
    const CGameEventPackage* package = reinterpret_cast<const CGameEventPackage*>(storage);
    const unsigned long counts[] = {0, 1, 39, 40, 0xffffffffUL};
    for (unsigned int i = 0; i < 5; ++i)
    {
        storage[1] = counts[i];
        if (package->GetNoEvents() != counts[i]) return 1;
    }
    printf("EVENT_PACKAGE_GET_NO_EVENTS_PASS cases=5\n");
    return 0;
}
