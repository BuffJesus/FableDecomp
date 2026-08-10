extern "C" void Ucopy_B_target();
__declspec(naked) void* Ucopy_B(void* a, void* b, void* c, void* d)
{
    (void)a; (void)b; (void)c; (void)d;
    __asm {
        push ebp
        mov ebp, esp
        lea eax, [ebp+0x0f]
        push eax
        push dword ptr [ebp+0x0c]
        push dword ptr [ebp+0x08]
        call Ucopy_B_target
        pop ebp
        ret 8
    }
}