struct CLandscapeBackgroundPatch {
    void** vft;
};

extern void __fastcall LBP_dtor(CLandscapeBackgroundPatch* self);
extern void __cdecl LBP_delete(void* p);
extern void* __cdecl LBP_new(unsigned int sz);
extern void __fastcall LBP_ctor(CLandscapeBackgroundPatch* self);

// `vector_deleting_destructor' for CLandscapeBackgroundPatch.
// The retail 50-byte slot is the vector-deleting-destructor thunk (bytes 0x00-0x1b,
// returns via `ret 4`) immediately followed by the class allocating/creation stub
// (bytes 0x1c-0x31, plain `ret`).  MSVC emits these two thunks back-to-back for the
// same class; the optimizer will not keep the second as unreachable C++, so the exact
// byte layout is reproduced with __declspec(naked) inline asm.  All call/jmp rel32
// targets are relocation-masked in the parity check.
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
        // --- adjacent creation/allocation stub (bytes 0x1c-0x31) ---
        push 0x58
        call LBP_new
        test eax, eax
        pop  ecx
        je   short L_zero
        mov  ecx, eax
        jmp  LBP_ctor
    L_zero:
        xor  eax, eax
        ret
    }
}