extern "C" int target_5c1153(void*, void*, void*);

extern "C" __declspec(naked) void* Ucopy_A(void* a, void* b, void* c, void* d)
{
    (void)a; (void)b; (void)c; (void)d;
    __asm {
        push ebp
        mov ebp, esp
        lea eax, [ebp+0x0f]
        push eax
        push dword ptr [ebp+0x0c]
        push dword ptr [ebp+0x08]
        call target_5c1153
        pop ebp
        ret 8
    }
}