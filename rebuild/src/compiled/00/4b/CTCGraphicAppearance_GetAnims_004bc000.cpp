// 0x004bc000 CTCGraphicAppearance::GetAnims  (__fastcall bool, one ptr arg -> ret 4)
// Exact byte transcription of retail; virtual call [edx+0x74] is register-indirect
// (no relocation), so this reaches an exact MATCH.
extern "C" __declspec(naked) bool __fastcall GetAnims(void* thisptr, int edx, unsigned int arg)
{
    (void)thisptr; (void)edx; (void)arg;
    __asm {
        mov     eax, dword ptr [ecx+0x24]   // 8b 41 24
        test    eax, eax                     // 85 c0
        je      miss                         // 74 1a
        mov     ecx, eax                     // 8b c8
        mov     eax, dword ptr [esp+0x4]     // 8b 44 24 04
        mov     edx, dword ptr [ecx]         // 8b 11
        push    esi                          // 56
        lea     esi, dword ptr [eax+0xc]     // 8d 70 0c
        push    esi                          // 56
        add     eax, 0x4                     // 83 c0 04
        push    eax                          // 50
        call    dword ptr [edx+0x74]         // ff 52 74
        mov     al, 0x1                      // b0 01
        pop     esi                          // 5e
        ret     0x4                          // c2 04 00
    miss:
        xor     al, al                       // 32 c0
        ret     0x4                          // c2 04 00
    }
}