// Behaviour fixture for C3DAnimationManager::Initialise / Uninitialise (provider C3DAnimationManager_Initialise_00a9d730.cpp).
// The class's base constructor is out of line elsewhere, so the methods run on raw storage laid out like retail:
// Initialise must store the bank at +0x04 only, Uninitialise must clear it, and neither may touch the vptr slot or
// the BoneIndicesBuffer that follows.
#include <stdio.h>
#include "fable_3d_animation_manager.h"

static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

int main()
{
    unsigned long raw[0x4c / 4];
    for (int i = 0; i < 0x4c / 4; ++i) raw[i] = 0xA5A50000u + i;
    C3DAnimationManager* manager = reinterpret_cast<C3DAnimationManager*>(raw);
    CMeshDataBank* bank = reinterpret_cast<CMeshDataBank*>(0x12345678);

    manager->Initialise(bank);
    CHECK(raw[1] == 0x12345678u);
    CHECK(raw[0] == 0xA5A50000u && raw[2] == 0xA5A50002u);
    manager->Initialise(0);
    CHECK(raw[1] == 0);
    manager->Initialise(bank);
    manager->Uninitialise();
    CHECK(raw[1] == 0);
    CHECK(raw[0] == 0xA5A50000u && raw[2] == 0xA5A50002u && raw[0x4c / 4 - 1] == 0xA5A50000u + 0x4c / 4 - 1);

    if (g_fail) { printf("ANIMATION_MANAGER_INITIALISE_FAIL %d\n", g_fail); return 1; }
    printf("ANIMATION_MANAGER_INITIALISE_OK\n");
    return 0;
}
