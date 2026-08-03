#include <cstdio>

struct AbilityDef {
    char pad[0x44];
    int  graphicIndex;   // +0x44
};

struct AbilityEntry {    // 8-byte stride
    int         pad0;    // +0
    AbilityDef* def;     // +4
};

struct CTCInventoryAbilities {
    char          pad[0x160];
    AbilityEntry* entries; // +0x160
};

int __fastcall GetAbilityGraphicIndex(CTCInventoryAbilities* self, void* /*edx*/, int index)
{
    return self->entries[index].def->graphicIndex;
}

static int call_it(CTCInventoryAbilities* self, int index)
{
    int result;
    __asm {
        mov  ecx, self
        push index
        call GetAbilityGraphicIndex
        mov  result, eax
    }
    return result;
}

int main()
{
    // Build a small array of 3 ability entries.
    AbilityDef defs[3];
    for (int i = 0; i < 3; ++i) defs[i].graphicIndex = 1000 + i;

    AbilityEntry entries[3];
    for (int i = 0; i < 3; ++i) { entries[i].pad0 = -1; entries[i].def = &defs[i]; }

    CTCInventoryAbilities obj;
    obj.entries = entries;

    int r0 = call_it(&obj, 0);
    int r1 = call_it(&obj, 1);
    int r2 = call_it(&obj, 2);

    if (r0 != 1000 || r1 != 1001 || r2 != 1002) {
        printf("FAIL r0=%d r1=%d r2=%d\n", r0, r1, r2);
        return 1;
    }
    printf("OK_0x005ede7f r0=%d r1=%d r2=%d\n", r0, r1, r2);
    return 0;
}