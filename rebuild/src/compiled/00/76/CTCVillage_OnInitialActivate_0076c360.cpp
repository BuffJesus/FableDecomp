// Byte-exact reconstruction of CTCVillage::OnInitialActivate @ 0x0076c360

struct Callee;

// The engine callee at 0x76c000: __fastcall (ecx, edx, stack arg)
extern void __fastcall Callee_Do(Callee* ecx, void* edx, unsigned char arg);

struct CTCVillage {
    Callee* field0;   // +0x00  -> passed in ecx
    void*   field4;   // +0x04  -> passed in edx
    // +0x08 padding
    char    pad8[4];
    unsigned char flagC; // +0x0c
    unsigned char flagD; // +0x0d
};

void __fastcall OnInitialActivate(CTCVillage* self)
{
    if (self->flagD) {
        Callee_Do(self->field0, self->field4, self->flagC);
        self->flagD = 0;
    }
}