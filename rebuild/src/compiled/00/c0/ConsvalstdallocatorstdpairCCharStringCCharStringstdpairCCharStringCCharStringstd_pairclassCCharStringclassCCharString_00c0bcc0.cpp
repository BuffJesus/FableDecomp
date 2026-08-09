extern "C" void (*g_Cons_val_target)(void*, void*, void*);

__declspec(naked) void _Cons_val_ctor(void*, void*, void*)
{
    __asm {
        mov eax, dword ptr [esp+4]
        test eax, eax
        jne L1
        ret 4
    L1:
        mov dword ptr [esp+4], eax
        jmp dword ptr [g_Cons_val_target]
    }
}