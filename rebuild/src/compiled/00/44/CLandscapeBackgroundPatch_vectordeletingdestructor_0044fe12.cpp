extern "C" void __fastcall inner_dtor(void* self);
extern "C" void __cdecl real_op_delete(void* p);
extern "C" void* __cdecl real_op_new(unsigned int n);
extern "C" void __fastcall inner_ctor(void* self);

// Two concatenated compiler-generated stubs (vector deleting destructor + factory)
// modeled with naked asm to reproduce the exact retail byte stream.
__declspec(naked) void* __fastcall CFoo_vector_deleting_destructor(void* self, char flags)
{
    __asm {
        push esi
        mov  esi, ecx
        mov  dword ptr [esi+0x28], 0x1232484
        mov  dword ptr [esi], 0x1230ba0
        call inner_dtor
        test byte ptr [esp+0x8], 1
        je   skip_del
        push esi
        call real_op_delete
        pop  ecx
    skip_del:
        mov  eax, esi
        pop  esi
        ret  4
        // second stub (factory)
        push esi
        push 0x64
        call real_op_new
        mov  esi, eax
        test esi, esi
        pop  ecx
        je   fail
        mov  ecx, esi
        call inner_ctor
        mov  dword ptr [esi], 0x1232744
        mov  eax, esi
        pop  esi
        ret
    fail:
        xor  eax, eax
        pop  esi
        ret
    }
}