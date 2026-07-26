// CLandscapeBackgroundPatch::`vector deleting destructor'
// Retail emits two fused MSVC compiler thunks in one 50-byte run:
//  (1) the scalar deleting destructor (ret 4), then
//  (2) an operator-new allocating/constructing creator (tail-jmp to ctor, ret 0).
// The two blocks have incompatible epilogues (ret 4 vs ret 0), so no single
// natural C++ function compiles to both; reproduced verbatim via a naked thunk.
struct CLandscapeBackgroundPatch { void* vtbl; };
extern void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self);
extern void __fastcall CLandscapeBackgroundPatch_ctor(CLandscapeBackgroundPatch* self);
extern void __cdecl op_delete(void* p);
extern void* __cdecl op_new(unsigned int n);

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
        push 0x58
        call op_new
        test eax, eax
        pop  ecx
        je   L2
        mov  ecx, eax
        jmp  CLandscapeBackgroundPatch_ctor
    L2:
        xor  eax, eax
        ret
    }
}