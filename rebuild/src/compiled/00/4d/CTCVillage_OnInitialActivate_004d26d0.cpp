// Byte-exact reconstruction of CTCVillage::OnInitialActivate @ 0x004d26d0

struct Callee;

// 0x4d2570: __fastcall(this=ecx, arg2=edx, stack byte)
extern "C" void __fastcall Callee_Do(Callee* self, void* arg2, unsigned char flag);

struct CTCVillage {
    Callee* m_pCallee;      // +0x00  -> ecx for the call
    void*   m_arg;          // +0x04  -> edx for the call
    // +0x08 gap
    unsigned char pad08;    // +0x08
    unsigned char pad09;    // +0x09
    unsigned char pad0a;    // +0x0a
    unsigned char pad0b;    // +0x0b
    unsigned char m_byteC;  // +0x0c  -> pushed byte
    unsigned char m_flagD;  // +0x0d  -> gate + cleared
};

void __fastcall OnInitialActivate(CTCVillage* self)
{
    if (self->m_flagD != 0)
    {
        Callee_Do(self->m_pCallee, self->m_arg, self->m_byteC);
        self->m_flagD = 0;
    }
}