struct CLandscapeBackgroundPatch {
    void** vft;
};

extern void __fastcall LBP_dtor(CLandscapeBackgroundPatch* self);
extern void __cdecl LBP_delete(void* p);

// `vector_deleting_destructor' for CLandscapeBackgroundPatch.
// The retail 28-byte slot is the vector-deleting-destructor thunk (bytes 0x00-0x1b,
// returns via `ret 4`).  All call rel32 targets are relocation-masked in the parity
// check.
__declspec(naked) void* __fastcall
CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx, unsigned int flags)
{
    (void)self; (void)edx; (void)flags;
    __asm {
        push esi
        mov  esi, ecx
        call LBP_dtor
        test byte ptr [esp+8], 1
        je   short L_ret
        push esi
        call LBP_delete
        pop  ecx
    L_ret:
        mov  eax, esi
        pop  esi
        ret  4
    }
}