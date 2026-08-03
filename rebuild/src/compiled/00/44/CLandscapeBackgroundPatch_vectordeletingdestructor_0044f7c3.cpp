// CLandscapeBackgroundPatch vector-deleting-destructor (34 bytes retail).
extern "C" void __fastcall CLBP_realdtor(void* self);
extern "C" void __cdecl CLBP_opdelete(void* p);

__declspec(naked) void* __fastcall vector_deleting_destructor(void* self, int edx_dummy, unsigned char flags)
{
    __asm {
        push    esi
        mov     esi, ecx
        mov     dword ptr [esi], 01230BA0h
        call    CLBP_realdtor
        test    byte ptr [esp+8], 1
        je      skip_delete
        push    esi
        call    CLBP_opdelete
        pop     ecx
    skip_delete:
        mov     eax, esi
        pop     esi
        ret     4
    }
}
