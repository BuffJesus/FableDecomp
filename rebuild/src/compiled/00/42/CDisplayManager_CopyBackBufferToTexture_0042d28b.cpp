struct CTexture;
struct CDisplayManager;
extern "C" void __fastcall DM_Grab(void);
__declspec(naked) CTexture* __fastcall CDisplayManager_CopyBackBufferToTexture(CDisplayManager* self)
{
    __asm {
        push ebp
        mov ebp, esp
        push ecx
        push esi
        lea eax, [ebp-1]
        push eax
        lea eax, [ebp-1]
        push eax
        mov esi, ecx
        call DM_Grab
        mov eax, esi
        pop esi
        leave
        ret
    }
}