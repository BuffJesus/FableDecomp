// CPlayer::IsTargetLocked  @ 0x004874a0  (__fastcall, this=ecx)
// Exact-byte transcription of the retail disassembly:
//   mov al,[ecx+0x20a]; test al,al; je L1
//   Lret1: mov al,1; ret
//   L1: mov al,[ecx+0x20b]; test al,al; jne Lret1
//   mov al,[ecx+0x20c]; test al,al; setne al; ret
__declspec(naked) bool IsTargetLocked()
{
    __asm {
        mov  al, byte ptr [ecx+0x20a]
        test al, al
        je   L1
    Lret1:
        mov  al, 1
        ret
    L1:
        mov  al, byte ptr [ecx+0x20b]
        test al, al
        jne  Lret1
        mov  al, byte ptr [ecx+0x20c]
        test al, al
        setne al
        ret
    }
}