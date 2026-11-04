#include "fable_game.h"
#include <stdio.h>
#include <string.h>

int main()
{
    unsigned long storage[sizeof(CNewFrontendGameComponent) / 4];
    unsigned char expected[sizeof(storage)];
    memset(storage, 0xA5, sizeof(storage));
    memcpy(expected, storage, sizeof(storage));
    expected[8] = 1;
    CNewFrontendGameComponent* component = reinterpret_cast<CNewFrontendGameComponent*>(storage);
    component->CNewFrontendGameComponent::SetQuit();
    if (false || memcmp(storage, expected, sizeof(storage)) != 0)
        return 1;
    puts("PASS_00430340");
    return 0;
}
