#include <cstdio>
#include <cstring>

struct AbilityEntry {
    int level;
    int pad;
};

struct CTCInventoryAbilities {
    char pad[0x160];
    AbilityEntry* abilities; // at +0x160
    int GetAbilityLevel(int index);
};

int CTCInventoryAbilities::GetAbilityLevel(int index) {
    return this->abilities[index].level;
}

// Invoke __fastcall member: this in ecx, arg on stack, callee cleans (ret 4).
static int call_it(CTCInventoryAbilities* self, int index) {
    int result;
    int (CTCInventoryAbilities::*mp)(int) = &CTCInventoryAbilities::GetAbilityLevel;
    void* fp = (void*&)mp;
    __asm {
        mov  ecx, self
        push index
        call fp
        mov  result, eax
    }
    return result;
}

int main() {
    CTCInventoryAbilities obj;
    memset(&obj, 0, sizeof(obj));

    AbilityEntry table[5];
    for (int i = 0; i < 5; ++i) { table[i].level = 100 + i * 7; table[i].pad = -1; }
    obj.abilities = table;

    int v0 = call_it(&obj, 0);
    int v3 = call_it(&obj, 3);
    int v4 = call_it(&obj, 4);

    if (v0 != 100) { printf("FAIL v0=%d\n", v0); return 1; }
    if (v3 != 121) { printf("FAIL v3=%d\n", v3); return 1; }
    if (v4 != 128) { printf("FAIL v4=%d\n", v4); return 1; }

    printf("OK_0x005ede93 v0=%d v3=%d v4=%d\n", v0, v3, v4);
    return 0;
}