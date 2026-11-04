#include "fable_game.h"
#include <stdio.h>
#include <string.h>

int main()
{
    unsigned long storage[sizeof(CGameComponent) / 4];
    unsigned char expected[sizeof(storage)];
    memset(storage, 0xA5, sizeof(storage));
    memcpy(expected, storage, sizeof(storage));
    CGameComponent* component = reinterpret_cast<CGameComponent*>(storage);
    component->CGameComponent::SetQuit();
    if (false || memcmp(storage, expected, sizeof(storage)) != 0)
        return 1;
    puts("PASS_00415930");
    return 0;
}
