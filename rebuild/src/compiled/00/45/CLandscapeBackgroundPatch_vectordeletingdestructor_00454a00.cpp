// CLandscapeBackgroundPatch::`vector deleting destructor'
// Real retail function is 28 bytes: the scalar deleting destructor thunk
// (call dtor; optional operator delete when flag&1; return this; ret 4).
// A fused operator-new creator block that followed it in the over-captured
// slice has been dropped.
struct CLandscapeBackgroundPatch { void* vtbl; };
extern void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self);
extern void __cdecl op_delete(void* p);

__declspec(naked) void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch*, int, unsigned int)
{
    __asm {
        push esi
        mov  esi, ecx
        call CLandscapeBackgroundPatch_dtor
        test byte ptr [esp+8], 1
        je   L1
        push esi
        call op_delete
        pop  ecx
    L1:
        mov  eax, esi
        pop  esi
        ret  4
    }
}
