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