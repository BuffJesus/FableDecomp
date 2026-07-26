// CLandscapeBackgroundPatch scalar-deleting-destructor + creator, laid out as one
// contiguous code region exactly as retail emitted them.
extern "C" void __fastcall CLBP_realdtor(void* self);
extern "C" void __cdecl CLBP_opdelete(void* p);
extern "C" void* __cdecl CLBP_opnew(unsigned int n);
extern "C" void __fastcall CLBP_ctor(void* self);

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
        // ---- creator (contiguous follow-on function) ----
        push    0C8h
        call    CLBP_opnew
        test    eax, eax
        pop     ecx
        je      new_failed
        mov     ecx, eax
        jmp     CLBP_ctor
    new_failed:
        xor     eax, eax
        ret
    }
}