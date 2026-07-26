#include <stdio.h>
#include <string.h>
#include "candidates/object_family_compiled.h"

CMainGameComponent* gMainGameComponent;

unsigned long __fastcall GFROR13(unsigned long value)
{
    return (value >> 13) | (value << 19);
}

int main()
{
    CMainGameComponent game;
    CObjectFamilyDef family;
    CObjectFamilyEntry entries[3];
    memset(&game, 0, sizeof(game));
    memset(&family, 0, sizeof(family));
    gMainGameComponent = &game;

    if (family.GetRandomObject() != 0) return 1;

    entries[0].Object = 101; entries[0].Probability = 0;
    entries[1].Object = 202; entries[1].Probability = 7;
    entries[2].Object = 303; entries[2].Probability = 0;
    family.mObjectsBegin = entries;
    family.mObjectsEnd = entries + 3;
    family.mObjectsCapacity = entries + 3;
    game.WorldSeed = 0x12345678U;
    if (family.GetRandomObject() != 202) return 2;
    if (game.WorldSeed != GFROR13(0x12345678U * 0x24A1U + 0x24DFU)) return 3;

    entries[1].Probability = 0;
    if (family.GetRandomObject() != 0) return 4;
    printf("OBJECT_FAMILY_RANDOM_TEST PASS\n");
    return 0;
}
