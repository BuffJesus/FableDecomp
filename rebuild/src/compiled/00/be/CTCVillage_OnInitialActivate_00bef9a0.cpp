// CTCVillage::OnInitialActivate  0x00bef9a0
struct CObj;

// callee at 0xbef5c0: __fastcall (ecx=obj, edx=edxarg, stack=byteval)
extern void __fastcall Callee(CObj* obj, void* edxarg, unsigned char v);

struct CTCVillage {
    CObj*  f0;       // +0x00 -> ecx
    void*  f4;       // +0x04 -> edx
    // +0x08
    int    pad8;
    unsigned char fC; // +0x0c
    unsigned char fD; // +0x0d
};

void __fastcall OnInitialActivate(CTCVillage* self)
{
    if (self->fD) {
        Callee(self->f0, self->f4, self->fC);
        self->fD = 0;
    }
}