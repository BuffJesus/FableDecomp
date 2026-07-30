__declspec(naked) void ConstructItemDescription(void *arg0, void *arg1)
{
    (void)arg0; (void)arg1;
    __asm
    {
        mov eax, dword ptr [ecx]
        push 0
        push dword ptr [esp+0x0c]
        push dword ptr [esp+0x0c]
        call dword ptr [eax+0x138]
        ret 8
    }
}