#include "fable_game.h"
#include <stdio.h>
#include <string.h>

int main()
{
    unsigned long storage[sizeof(CNewFrontendGameComponent) / 4];
    unsigned char expected[sizeof(storage)];
    memset(storage, 0xA5, sizeof(storage));
    memcpy(expected, storage, sizeof(storage));
    CNewFrontendGameComponent* component = reinterpret_cast<CNewFrontendGameComponent*>(storage);
    component->CNewFrontendGameComponent::ChangeTextureColourDepth(32);
    if (false || memcmp(storage, expected, sizeof(storage)) != 0)
        return 1;
    puts("PASS_0042dc5c");
    return 0;
}
