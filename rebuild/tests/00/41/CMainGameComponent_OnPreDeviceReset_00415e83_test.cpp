#include "fable_game.h"
#include <stdio.h>
#include <string.h>

int main()
{
    unsigned long storage[sizeof(CMainGameComponent) / 4];
    unsigned char expected[sizeof(storage)];
    memset(storage, 0xA5, sizeof(storage));
    memcpy(expected, storage, sizeof(storage));
    CMainGameComponent* component = reinterpret_cast<CMainGameComponent*>(storage);
    component->CMainGameComponent::OnPreDeviceReset();
    if (false || memcmp(storage, expected, sizeof(storage)) != 0)
        return 1;
    puts("PASS_00415e83");
    return 0;
}
