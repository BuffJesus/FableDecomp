extern "C" void dtor1();
extern "C" void ctor2();
extern "C" void opdelete();
extern "C" void opnew();
extern int vtbl1;
extern int vtbl2;

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
        push esi
        push 78h
        call opnew
        mov  esi, eax
        test esi, esi
        pop  ecx
        je   L42
        mov  ecx, esi
        call ctor2
        mov  dword ptr [esi], offset vtbl2
        mov  eax, esi
        pop  esi
        ret
    L42:
        xor  eax, eax
        pop  esi
        ret
    }
}