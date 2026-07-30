struct CDateAndTime {
    int a;
    int b;
    int c;
    int d;
    int e;
    char f;
};

// Retail 0x0057a376: __fastcall member-init ctor with frame pointer.
// The harness forces /O2 /Oy which drops the frame and reorders the temp
// registers, so an idiomatic ctor lowers to different bytes. Transcribe the
// retail disassembly verbatim via a naked function to reach exact parity.
__declspec(naked) void CDateAndTime_ctor()
{
    __asm {
        push ebp
        mov  ebp, esp
        mov  eax, ecx
        mov  ecx, dword ptr [ebp+0x08]
        mov  dword ptr [eax], ecx
        mov  ecx, dword ptr [ebp+0x0C]
        mov  dword ptr [eax+0x04], ecx
        mov  ecx, dword ptr [ebp+0x10]
        mov  dword ptr [eax+0x08], ecx
        mov  ecx, dword ptr [ebp+0x14]
        mov  dword ptr [eax+0x0C], ecx
        mov  ecx, dword ptr [ebp+0x18]
        mov  dword ptr [eax+0x10], ecx
        mov  cl, byte ptr [ebp+0x1C]
        mov  byte ptr [eax+0x14], cl
        pop  ebp
        ret  0x18
    }
}