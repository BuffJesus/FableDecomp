// Combined MSVC compiler thunks emitted contiguously for CLandscapeBackgroundPatch:
//   [0x00..0x21] `vector deleting destructor' (this in ecx, __flags at [esp+4] after push)
//   [0x22..0x45] allocating default constructor closure (operator new(0x54) + ctor)
// Reproduced as a single naked routine so the emitted .text is byte-exact.

extern void CLandscapeBackgroundPatch_dtor(void);   // 0x5ac593
extern void engine_free(void);                      // 0x7ae9ff  operator delete
extern void engine_new(void);                        // 0x7aea5d  operator new
extern void CLandscapeBackgroundPatch_ctor(void);    // ctor (rel from 0x33)
extern void* g_vtbl_dtor;                            // 0x1230ba0
extern void* g_vtbl_ctor;                            // 0x12329b4

__declspec(naked) void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor()
{
    __asm {
        push esi
        mov  esi, ecx
        mov  dword ptr [esi], offset g_vtbl_dtor
        call CLandscapeBackgroundPatch_dtor
        test byte ptr [esp+8], 1
        je   skip_free
        push esi
        call engine_free
        pop  ecx
    skip_free:
        mov  eax, esi
        pop  esi
        ret  4

        // --- allocating default constructor closure ---
        push esi
        push 54h
        call engine_new
        mov  esi, eax
        test esi, esi
        pop  ecx
        je   alloc_fail
        mov  ecx, esi
        call CLandscapeBackgroundPatch_ctor
        mov  dword ptr [esi], offset g_vtbl_ctor
        mov  eax, esi
        pop  esi
        ret
    alloc_fail:
        xor  eax, eax
        pop  esi
        ret
    }
}