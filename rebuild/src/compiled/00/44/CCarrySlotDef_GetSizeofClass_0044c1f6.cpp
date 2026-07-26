struct CCarrySlotDef {
    unsigned long _first;
};

extern "C" void __cdecl CCarrySlotDef_sub_helper(void);

__declspec(naked) unsigned long __fastcall CCarrySlotDef_GetSizeofClass(CCarrySlotDef* self)
{
    (void)self;
    __asm {
        push 0x38
        pop  eax
        ret
        mov  eax, ecx
        and  dword ptr [eax], 0
        ret
        push esi
        push 0xffffffff
        push 0x1231f68
        mov  esi, ecx
        call CCarrySlotDef_sub_helper
        mov  eax, esi
        pop  esi
        ret
    }
}