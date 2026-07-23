extern "C" int g_vtbl_a;
extern "C" int g_vtbl_b;
extern "C" void dtor_fn();
extern "C" void delete_fn();
extern "C" void new_fn();
extern "C" void ctor_fn();

__declspec(naked) void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(void* self, unsigned int flags)
{
    __asm {
        push esi
        mov  esi, ecx
        mov  dword ptr [esi+0x28], offset g_vtbl_a
        mov  dword ptr [esi], offset g_vtbl_b
        call dtor_fn
        test byte ptr [esp+8], 1
        je   L1
        push esi
        call delete_fn
        pop  ecx
    L1:
        mov  eax, esi
        pop  esi
        ret  4
        push 0x48
        call new_fn
        test eax, eax
        pop  ecx
        je   L2
        mov  ecx, eax
        jmp  ctor_fn
    L2:
        xor  eax, eax
        ret
    }
}