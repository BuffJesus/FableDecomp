#include <stdio.h>
__declspec(naked) bool IsAbilityAvailable() {
    __asm {
        mov eax, dword ptr [ecx+0x160]
        mov edx, dword ptr [esp+4]
        xor ecx, ecx
        cmp dword ptr [eax+edx*8], ecx
        setge cl
        mov al, cl
        ret 4
    }
}

struct AbilitySlot { int value; int pad; };
struct CTCInventoryAbilities {
    char pad[0x160];
    AbilitySlot* m_abilities;
};

static bool call_it(CTCInventoryAbilities* self, int index) {
    unsigned char r;
    void* fn = (void*)&IsAbilityAvailable;
    __asm {
        mov ecx, self
        mov eax, index
        push eax
        call fn
        mov r, al
    }
    return r != 0;
}

int main() {
    AbilitySlot slots[4];
    slots[0].value = 5;   slots[0].pad = 0;
    slots[1].value = -1;  slots[1].pad = 0;
    slots[2].value = 0;   slots[2].pad = 0;
    slots[3].value = -100; slots[3].pad = 0;
    CTCInventoryAbilities obj;
    obj.m_abilities = slots;
    bool r0 = call_it(&obj, 0);
    bool r1 = call_it(&obj, 1);
    bool r2 = call_it(&obj, 2);
    bool r3 = call_it(&obj, 3);
    if (r0 && !r1 && r2 && !r3) {
        printf("OK_0x005edf08\n");
        return 0;
    }
    printf("BAD r0=%d r1=%d r2=%d r3=%d\n", r0, r1, r2, r3);
    return 1;
}