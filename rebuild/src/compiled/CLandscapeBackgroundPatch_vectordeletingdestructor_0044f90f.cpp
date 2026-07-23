struct CLandscapeBackgroundPatch {
    void* vptr;
};
extern void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self);
extern void __cdecl op_delete(void* p);
extern void* __cdecl op_new(unsigned int n);
extern void __fastcall CLandscapeBackgroundPatch_ctor(void* self);
extern void* const CLandscapeBackgroundPatch_vftable;

// vector deleting destructor slice: scalar-deleting-destructor body, immediately
// followed by the adjacent allocating-constructor thunk (both captured in the
// 56-byte retail slice). Emitted as naked asm to reproduce the exact bytes.
__declspec(naked) void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, unsigned int flags)
{
    (void)self; (void)flags;
    __asm {
        // --- scalar deleting destructor (this in ecx, flags at [esp+8]) ---
        push    esi
        mov     esi, ecx
        mov     dword ptr [esi], offset CLandscapeBackgroundPatch_vftable
        call    CLandscapeBackgroundPatch_dtor
        test    byte ptr [esp+8], 1
        je      skip_delete
        push    esi
        call    op_delete
        pop     ecx
    skip_delete:
        mov     eax, esi
        pop     esi
        ret     4
        // --- adjacent allocating ctor thunk ---
        push    3ch
        call    op_new
        test    eax, eax
        pop     ecx
        je      alloc_fail
        mov     ecx, eax
        jmp     CLandscapeBackgroundPatch_ctor
    alloc_fail:
        xor     eax, eax
        ret
    }
}