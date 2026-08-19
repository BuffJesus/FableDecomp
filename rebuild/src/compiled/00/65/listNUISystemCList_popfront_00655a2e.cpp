extern "C" void helper_53c7b9(void);

extern "C" void __declspec(naked) pop_front_UIList(void)
{
    __asm {
        push ecx
        mov edx, dword ptr [ecx]
        mov edx, dword ptr [edx]
        push ecx
        mov eax, esp
        mov dword ptr [eax], edx
        lea eax, [esp+4]
        push eax
        call helper_53c7b9
        pop ecx
        ret
    }
}

extern "C" void __declspec(naked) helper_53c7b9(void)
{
    __asm { ret }
}