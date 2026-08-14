// CTCVillage::OnInitialActivate @ 0x00a420c0

struct CTCVillage {
    void*         f0;     // +0x00  -> ecx to callee
    void*         f4;     // +0x04  -> edx to callee (arg1)
    char          pad8[4];
    bool          byteC;  // +0x0c  pending flag/mode passed to callee
    unsigned char flagD;  // +0x0d  "needs-initial-activate" latch
};

// callee at 0x00a41e20: __fastcall, ecx = f0, edx = f4, stack arg = byteC
extern void __fastcall Village_Sub(void* ecx, void* edx, bool arg);

void __fastcall OnInitialActivate(CTCVillage* self)
{
    if (self->flagD) {
        Village_Sub(self->f0, self->f4, self->byteC);
        self->flagD = 0;
    }
}