// CTCVillage::OnInitialActivate @ 0x004d2690
// __fastcall (self in ecx) modeled as __fastcall free fn with self in ecx.

struct CInner;

// Callee at 0x4d2470: __fastcall member taking (ecx=obj, edx=arg1, byte arg2)
// Modeled as __fastcall: ecx=obj, edx=arg1, pushed byte.
void __fastcall Inner_Do(CInner* obj, int arg1, unsigned char b);

struct CTCVillage {
    CInner* obj;        // +0x00
    int     field4;     // +0x04
    // +0x08 unknown
    char    pad8;       // +0x08
    char    pad9;       // +0x09
    char    padA;       // +0x0a
    char    padB;       // +0x0b
    unsigned char byteC; // +0x0c
    char    flagD;      // +0x0d
};

void __fastcall OnInitialActivate(CTCVillage* self)
{
    if (self->flagD) {
        Inner_Do(self->obj, self->field4, self->byteC);
        self->flagD = 0;
    }
}