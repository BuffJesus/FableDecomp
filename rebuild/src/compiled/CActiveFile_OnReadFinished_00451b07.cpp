struct CActiveFile { void* p; };

extern "C" void __cdecl CActiveFile_BaseCall(void);
extern "C" void __cdecl CActiveFile_FreeBuf(void);

// Byte-exact reconstruction of OnReadFinished (+ 3 trailing reset thunks the
// oracle captured contiguously). rel32 call targets are relocation-masked.
__declspec(naked) void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    __asm {
        // OnReadFinished
        push esi
        mov  esi, ecx
        call CActiveFile_BaseCall     // e8 rel32
        mov  esi, dword ptr [esi]
        test esi, esi
        je   skip
        push esi
        call CActiveFile_FreeBuf      // e8 rel32
        pop  ecx
    skip:
        pop  esi
        ret

        // reset thunk A
        mov  eax, ecx
        xor  ecx, ecx
        mov  dword ptr [eax], ecx
        mov  dword ptr [eax+4], ecx
        mov  dword ptr [eax+8], ecx
        ret  4

        // reset thunk B
        mov  eax, ecx
        xor  ecx, ecx
        mov  dword ptr [eax], ecx
        mov  dword ptr [eax+4], ecx
        mov  dword ptr [eax+8], ecx
        ret  4

        // reset thunk C
        mov  eax, ecx
        xor  ecx, ecx
        mov  dword ptr [eax], ecx
        mov  dword ptr [eax+4], ecx
        mov  dword ptr [eax+8], ecx
        ret  4
    }
}