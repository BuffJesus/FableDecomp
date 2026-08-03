// CLandscapeBackgroundPatch::`vector deleting destructor' (this in ecx, __flags at [esp+4] after push)
// Reproduced as a naked routine so the emitted .text is byte-exact (34 bytes).
extern void CLandscapeBackgroundPatch_dtor(void);   // 0x5ac593
extern void engine_free(void);                      // 0x7ae9ff  operator delete
extern void* g_vtbl_dtor;                           // 0x1230ba0

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
    }
}
