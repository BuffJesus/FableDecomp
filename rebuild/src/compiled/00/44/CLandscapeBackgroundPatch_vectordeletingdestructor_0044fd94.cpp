// CLandscapeBackgroundPatch `vector deleting destructor'
// Fused compiler thunk: scalar-deleting destructor (ret 4) followed by the
// class default-constructor closure (ret). Emitted verbatim via naked asm so
// the two adjacent compiler-generated bodies land in one 63-byte range.
struct CLandscapeBackgroundPatch;

extern "C" void __fastcall CLandscapeBackgroundPatch_dtor_inner(CLandscapeBackgroundPatch* self);
extern "C" void __cdecl CLandscapeBackgroundPatch_op_delete(void* p);
extern "C" void* __cdecl CLandscapeBackgroundPatch_op_new(unsigned int n);
extern "C" void __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor_ctor(CLandscapeBackgroundPatch* self);

__declspec(naked) void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, unsigned int flags)
{
    __asm {
        push    esi
        mov     esi, ecx
        mov     dword ptr [esi+0x28], 0x1232484
        mov     dword ptr [esi], 0x1230ba0
        call    CLandscapeBackgroundPatch_dtor_inner
        test    byte ptr [esp+8], 1
        je      L_nodel
        push    esi
        call    CLandscapeBackgroundPatch_op_delete
        pop     ecx
    L_nodel:
        mov     eax, esi
        pop     esi
        ret     4

        push    0x44
        call    CLandscapeBackgroundPatch_op_new
        test    eax, eax
        pop     ecx
        je      L_null
        mov     ecx, eax
        jmp     CLandscapeBackgroundPatch_vector_deleting_destructor_ctor
    L_null:
        xor     eax, eax
        ret
    }
}