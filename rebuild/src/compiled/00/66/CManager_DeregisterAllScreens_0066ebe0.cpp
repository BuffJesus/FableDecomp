extern "C" void __fastcall
CManager_DeregisterAllScreens_0066ebe0_InitialiseScrollIndices(
    void* manager, void*, long count);

extern "C" __declspec(naked) void __fastcall
CManager_DeregisterAllScreens_0066ebe0(void* manager, void*)
{
    __asm
    {
        mov edx, [ecx + 8]
        sub edx, [ecx + 4]
        xor eax, eax
        sar edx, 2
        je initialise

    clear:
        mov edx, [ecx + 4]
        and dword ptr [edx + eax * 4], 0
        mov edx, [ecx + 8]
        sub edx, [ecx + 4]
        inc eax
        sar edx, 2
        cmp eax, edx
        jb clear

    initialise:
        push 0
        call CManager_DeregisterAllScreens_0066ebe0_InitialiseScrollIndices
        ret
    }
}
