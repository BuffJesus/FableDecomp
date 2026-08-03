// CLandscapeBackgroundPatch `vector deleting destructor'
// Real 41-byte scalar-deleting destructor (ret 4). The trailing fused
// default-constructor closure that padded this to 63 bytes was over-capture
// and has been dropped.
struct CLandscapeBackgroundPatch;

extern "C" void __fastcall CLandscapeBackgroundPatch_dtor_inner(CLandscapeBackgroundPatch* self);
extern "C" void __cdecl CLandscapeBackgroundPatch_op_delete(void* p);

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
    }
}