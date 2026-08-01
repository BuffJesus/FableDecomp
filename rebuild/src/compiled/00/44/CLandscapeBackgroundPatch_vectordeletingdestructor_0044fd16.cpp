extern "C" void CLBP_dtor(void);
extern "C" void op_delete(void);
extern int g_vtbl0;
extern int g_vtbl28;
__declspec(naked) void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(void* self, int flags) {
    __asm {
        push esi
        mov esi, ecx
        mov dword ptr [esi+0x28], offset g_vtbl28
        mov dword ptr [esi], offset g_vtbl0
        call CLBP_dtor
        test byte ptr [esp+8], 1
        je skip_del
        push esi
        call op_delete
        pop ecx
    skip_del:
        mov eax, esi
        pop esi
        ret 4
    }
}
