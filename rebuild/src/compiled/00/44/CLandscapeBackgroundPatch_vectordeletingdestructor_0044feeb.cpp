
extern "C" void __fastcall CLBP_dtor(void* self);
extern "C" void __cdecl eng_delete(void*);
extern "C" void* __cdecl eng_new(unsigned int);
extern "C" void __fastcall CLBP_ctor(void* self);
extern int vtbl_a;
extern int vtbl_b;

__declspec(naked) void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(void* self, unsigned int flags)
{
    __asm {
        push esi
        mov  esi, ecx
        mov  dword ptr [esi], offset vtbl_a
        call CLBP_dtor
        test byte ptr [esp+8], 1
        je   L1
        push esi
        call eng_delete
        pop  ecx
    L1:
        mov  eax, esi
        pop  esi
        ret  4
    B2:
        push esi
        push 4Ch
        call eng_new
        mov  esi, eax
        test esi, esi
        pop  ecx
        je   L3
        mov  ecx, esi
        call CLBP_ctor
        mov  dword ptr [esi], offset vtbl_b
        mov  eax, esi
        pop  esi
        ret
    L3:
        xor  eax, eax
        pop  esi
        ret
    }
}