__declspec(naked) void SetObjectiveAsCompleted(void)
{
    __asm {
        mov ecx, dword ptr [ecx+0x74]
        mov eax, dword ptr [ecx]
        cmp eax, ecx
        je short L_ret
        mov edx, dword ptr [esp+0x4]
        _emit 0x8d
        _emit 0x49
        _emit 0x00
    L_loop:
        cmp dword ptr [eax+0x0c], edx
        je short L_set
        mov eax, dword ptr [eax]
        cmp eax, ecx
        jne short L_loop
        ret 0x4
    L_set:
        mov dword ptr [eax+0x10], 0x1
    L_ret:
        ret 0x4
    }
}