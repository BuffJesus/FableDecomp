extern "C" void __fastcall inner_dtor(void* self);
extern "C" void __cdecl real_op_delete(void* p);

// CLandscapeBackgroundPatch::`vector_deleting_destructor'
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
    }
}
