extern "C" void * Ucopy_target(void *a, void *b, void *c);
__declspec(naked) void * Ucopy_C(void *, void *, void *, void *)
{
    __asm {
        push ebp
        mov ebp, esp
        lea eax, [ebp+0x0f]
        push eax
        push dword ptr [ebp+0x0c]
        push dword ptr [ebp+0x08]
        call Ucopy_target
        pop ebp
        ret 8
    }
}