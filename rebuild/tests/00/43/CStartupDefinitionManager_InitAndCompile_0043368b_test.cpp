// Behaviour of CStartupDefinitionManager::InitAndCompile 0x0043368B: returns
// without touching the manager.
#include "fable_definition_manager.h"
#include <stdio.h>
#include <string.h>

int main()
{
    unsigned char storage[sizeof(CStartupDefinitionManager)];
    unsigned char expected[sizeof(storage)];
    for (size_t i = 0; i < sizeof(storage); ++i)
        storage[i] = static_cast<unsigned char>(i * 7 + 3);
    memcpy(expected, storage, sizeof(storage));
    CStartupDefinitionManager* manager = reinterpret_cast<CStartupDefinitionManager*>(storage);
    manager->CStartupDefinitionManager::InitAndCompile();
    if (memcmp(storage, expected, sizeof(storage)) != 0) return 1;
    puts("PASS_0043368b");
    return 0;
}
