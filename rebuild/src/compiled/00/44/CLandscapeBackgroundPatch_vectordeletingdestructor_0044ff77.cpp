extern "C" void dtor1();
extern "C" void opdelete();
extern int vtbl1;

__declspec(naked) void* vector_deleting_destructor()
{
    __asm {
        push esi
        mov  esi, ecx
        mov  dword ptr [esi], offset vtbl1
        call dtor1
        test byte ptr [esp+8], 1
        je   L1c
        push esi
        call opdelete
        pop  ecx
    L1c:
        mov  eax, esi
        pop  esi
        ret  4
    }
}
