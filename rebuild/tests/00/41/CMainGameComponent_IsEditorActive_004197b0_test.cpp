#include <stdio.h>
#include <string.h>
#include "fable_game.h"

int main(void)
{
    static union
    {
        double align;
        unsigned char bytes[sizeof(CMainGameComponent)];
    } storage;
    memset(storage.bytes, 0xFF, sizeof(storage.bytes));
    const CMainGameComponent* component =
        reinterpret_cast<const CMainGameComponent*>(storage.bytes);
    // Non-virtual call: the release body ignores the object entirely.
    if (component->CMainGameComponent::IsEditorActive())
        return 1;
    if (storage.bytes[0] != 0xFF || storage.bytes[sizeof(storage.bytes) - 1] != 0xFF)
        return 2;
    printf("PASS_004197b0\n");
    return 0;
}
