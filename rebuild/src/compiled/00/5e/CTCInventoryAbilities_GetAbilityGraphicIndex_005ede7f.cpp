#pragma optimize("s",on)
// GetAbilityGraphicIndex @ 0x005ede7f (CTCInventoryAbilities)
// __fastcall(int index):
//   mov eax,[ecx+0x160]     ; entries array base
//   mov ecx,[esp+4]         ; index
//   mov eax,[eax+ecx*8+4]   ; entries[index].def (stride 8, ptr at +4)
//   mov eax,[eax+0x44]      ; def->graphicIndex
//   ret 4

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