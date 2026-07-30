// external recalc routine that the retail function tail-jumps to
extern "C" void CTCHeroStats_RecalcRenown();

__declspec(naked) void UpgradeRenownLevel()
{
    __asm {
        mov  eax, [ecx+0x74]
        inc  dword ptr [ecx+0x70]
        cmp  [ecx+0x70], eax
        jle  short skip
        mov  [ecx+0x70], eax
    skip:
        and  dword ptr [ecx+0x78], 0
        jmp  CTCHeroStats_RecalcRenown
    }
}