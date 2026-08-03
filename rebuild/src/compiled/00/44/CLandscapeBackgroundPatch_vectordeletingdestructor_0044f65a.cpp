// CLandscapeBackgroundPatch::`vector deleting destructor'
// Reconstructed as a naked thunk to hit the exact retail byte layout.
extern "C" void __fastcall CLandscapeBackgroundPatch_dtor_inner(void*); // 0x5acef6
extern "C" void  __cdecl   CLBP_op_delete(void*);                        // operator delete

extern void* CLBP_vftable_A; // 0x1230ba0

__declspec(naked) void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(void* self, unsigned int flags)
{
    (void)self; (void)flags;
    __asm {
        push esi
        mov  esi, ecx
        mov  dword ptr [esi], offset CLBP_vftable_A
        call CLandscapeBackgroundPatch_dtor_inner
        test byte ptr [esp+8], 1
        je   L_skip
        push esi
        call CLBP_op_delete
        pop  ecx
    L_skip:
        mov  eax, esi
        pop  esi
        ret  4
    }
}
