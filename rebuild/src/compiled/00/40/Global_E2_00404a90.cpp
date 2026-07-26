extern unsigned long _DAT_013b7cec;
extern unsigned long _DAT_013b7cfc;
extern unsigned long _DAT_013b7ce4;
extern unsigned long _DAT_013b7ce8;
extern unsigned char DAT_013b7cf0;
extern unsigned char DAT_013b7cf1;
extern unsigned char DAT_013b7cf2;
extern unsigned long _DAT_013b7cf4;
extern unsigned long _DAT_013b7cf8;
extern unsigned long _DAT_013b7d00;
extern unsigned long _DAT_013b7d04;
extern unsigned long _DAT_013b7d08;
extern unsigned long _DAT_013b7d0c;
extern unsigned long _DAT_013b7d10;
extern unsigned long _DAT_013b7d14;
extern unsigned char DAT_013b7d18;
extern unsigned long _DAT_013b7d1c;
extern unsigned char DAT_013b7d20;
extern unsigned char DAT_013b7d21;

__declspec(naked) void __cdecl $E2()
{
    __asm
    {
        mov ecx, 0ff000000h
        xor eax, eax
        mov dword ptr [_DAT_013b7cec], ecx
        mov dword ptr [_DAT_013b7cfc], ecx
        mov ecx, 000ff0000h
        mov dword ptr [_DAT_013b7ce4], eax
        mov dword ptr [_DAT_013b7ce8], 00404040h
        mov byte ptr [DAT_013b7cf0], 018h
        mov byte ptr [DAT_013b7cf1], 0a0h
        mov byte ptr [DAT_013b7cf2], al
        mov dword ptr [_DAT_013b7cf4], eax
        mov dword ptr [_DAT_013b7cf8], 0ffffffffh
        mov dword ptr [_DAT_013b7d00], eax
        mov dword ptr [_DAT_013b7d04], 000ffff00h
        mov dword ptr [_DAT_013b7d08], 0000000ffh
        mov dword ptr [_DAT_013b7d0c], 00000ff00h
        mov dword ptr [_DAT_013b7d10], ecx
        mov dword ptr [_DAT_013b7d14], ecx
        mov byte ptr [DAT_013b7d18], 080h
        mov dword ptr [_DAT_013b7d1c], eax
        mov byte ptr [DAT_013b7d20], 2
        mov byte ptr [DAT_013b7d21], 1
        ret
    }
}
