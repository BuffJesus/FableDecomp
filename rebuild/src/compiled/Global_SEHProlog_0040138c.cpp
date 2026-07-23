extern "C" __declspec(naked) void __cdecl __SEH_prolog(unsigned long, long)
{
    __asm {
        push 004013D8h
        mov eax, dword ptr fs:[0]
        push eax
        mov eax, dword ptr [esp + 10h]
        mov dword ptr [esp + 10h], ebp
        lea ebp, [esp + 10h]
        sub esp, eax
        push ebx
        push esi
        push edi
        mov eax, dword ptr [ebp - 8]
        mov dword ptr [ebp - 18h], esp
        push eax
        mov eax, dword ptr [ebp - 4]
        mov dword ptr [ebp - 4], 0FFFFFFFFh
        mov dword ptr [ebp - 8], eax
        lea eax, [ebp - 10h]
        mov dword ptr fs:[0], eax
        ret
    }
}
