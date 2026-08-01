struct CActiveFile { void* p; };

extern "C" void __cdecl CActiveFile_BaseCall(void);
extern "C" void __cdecl CActiveFile_FreeBuf(void);

// Byte-exact reconstruction of OnReadFinished. rel32 call targets are
// relocation-masked. Trailing reset thunks were over-captured; dropped.
__declspec(naked) void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    __asm {
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
    }
}