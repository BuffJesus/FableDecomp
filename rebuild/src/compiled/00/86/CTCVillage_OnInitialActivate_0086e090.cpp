// CTCVillage::OnInitialActivate  0x0086e090
struct CTCVillage;

// callee 0x86dd80: __fastcall(ecx, edx, stack byte)
extern int __fastcall sub_86dd80(void* ecx, void* edx, unsigned char b);

struct CTCVillage {
    void* m0;     // +0x00
    void* m4;     // +0x04
    unsigned char m8;   // +0x08
    unsigned char m9;
    unsigned char ma;
    unsigned char mb;
    unsigned char mc;   // +0x0c
    unsigned char md;   // +0x0d
};

void __fastcall OnInitialActivate(CTCVillage* self)
{
    if (self->md) {
        sub_86dd80(self->m0, self->m4, self->mc);
        self->md = 0;
    }
}