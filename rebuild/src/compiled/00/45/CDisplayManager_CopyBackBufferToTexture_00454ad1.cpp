struct CTexture;
struct CDisplayManager {
    unsigned char _pad_0x0[0x54];
    void*** obj54;
    unsigned int f5c;
    unsigned int f60;
    unsigned int f1c4;
    int field1c4;
};

extern void DM_Helper(void);
extern void DM_Tail(void);

__declspec(naked) CTexture* __fastcall CDisplayManager_CopyBackBufferToTexture(CDisplayManager* self)
{
    __asm {
        push ebp
        mov  ebp, esp
        push ecx
        push esi
        lea  eax, [ebp-1]
        push eax
        lea  eax, [ebp-1]
        push eax
        mov  esi, ecx
        call DM_Helper
        mov  eax, esi
        pop  esi
        leave
        ret
        jmp  DM_Tail
    }
}