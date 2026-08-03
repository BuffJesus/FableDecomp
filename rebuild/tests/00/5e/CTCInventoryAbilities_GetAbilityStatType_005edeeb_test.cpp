#include <stdio.h>

struct Entry { void* a; struct Ability* b; };
struct Ability { char pad[0x40]; int statType; };
struct CTCInventoryAbilities { char pad[0x160]; Entry* table; };

int __fastcall GetAbilityStatType(CTCInventoryAbilities* self, void*, int index)
{
    Ability* ab = self->table[index].b;
    if (ab)
        return ab->statType;
    return 2;
}

int call_it(CTCInventoryAbilities* self, int index)
{
    int result;
    void* pself = self;
    __asm {
        push index
        mov  ecx, pself
        call GetAbilityStatType
        mov  result, eax
    }
    return result;
}

int main()
{
    Ability ab1;
    ab1.statType = 0x1234;

    Entry entries[3];
    entries[0].b = 0;
    entries[1].b = &ab1;
    entries[2].b = 0;

    CTCInventoryAbilities obj;
    *(Entry**)((char*)&obj + 0x160) = entries;

    int r0 = call_it(&obj, 0);
    int r1 = call_it(&obj, 1);

    if (r0 == 2 && r1 == 0x1234) {
        printf("OK_0x005edeeb\n");
        return 0;
    }
    printf("FAIL r0=%d r1=%d\n", r0, r1);
    return 1;
}