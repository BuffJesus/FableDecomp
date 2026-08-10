__declspec(naked) bool IsOptional() {
    __asm {
        cmp dword ptr [ecx+0x14], -1
        jg  L_true
        mov eax, dword ptr [ecx+0x0c]
        mov cl, byte ptr [eax+0x58]
        test cl, cl
        jne L_false
    L_true:
        mov al, 1
        ret
    L_false:
        xor al, al
        ret
    }
}