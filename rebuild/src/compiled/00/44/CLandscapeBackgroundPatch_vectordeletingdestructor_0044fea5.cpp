extern "C" void _dtor_body();
extern "C" void _op_delete();
extern "C" void _op_new();
extern "C" void _ctor_body();

__declspec(naked) void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(void* self, int edx, unsigned char flags)
{
    __asm {
        push esi
        mov  esi, ecx
        mov  dword ptr [esi], 0x1230ba0
        call _dtor_body
        test byte ptr [esp+8], 1
        je   L1
        push esi
        call _op_delete
        pop  ecx
    L1:
        mov  eax, esi
        pop  esi
        ret  4
        push esi
        push 0x2c
        call _op_new
        mov  esi, eax
        test esi, esi
        pop  ecx
        je   L2
        mov  ecx, esi
        call _ctor_body
        mov  dword ptr [esi], 0x1232814
        mov  eax, esi
        pop  esi
        ret
    L2:
        xor  eax, eax
        pop  esi
        ret
    }
}