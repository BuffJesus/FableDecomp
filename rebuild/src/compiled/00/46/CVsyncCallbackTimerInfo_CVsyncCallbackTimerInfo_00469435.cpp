struct CVsyncCallbackTimerInfo {
    void* p0;      // +0x00
    void* p4;      // +0x04
    float f8;      // +0x08
    void* pc;      // +0x0c
    float f10;     // +0x10
};

// __fastcall ctor: this=ecx, zero-inits all fields, returns this in eax.
// Idiomatic form folds the 0.0f float stores to integer moves, so transcribe
// the retail x87 sequence directly for exact byte parity.
__declspec(naked) CVsyncCallbackTimerInfo* __fastcall CVsyncCallbackTimerInfo(CVsyncCallbackTimerInfo* /*self ecx*/)
{
    __asm {
        mov  eax, ecx
        fldz
        xor  ecx, ecx
        mov  dword ptr [eax], ecx
        fstp dword ptr [eax+8]
        fldz
        mov  dword ptr [eax+4], ecx
        fstp dword ptr [eax+0x10]
        mov  dword ptr [eax+0xc], ecx
        ret
    }
}