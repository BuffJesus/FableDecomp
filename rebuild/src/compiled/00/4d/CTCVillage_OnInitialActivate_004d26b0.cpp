// CTCVillage::OnInitialActivate  0x004d26b0

struct SubObj;

// callee at 0x4d24f0: __fastcall (ecx=this, edx=arg1, stack: arg2 byte)
void __fastcall Sub_Call(SubObj* self, int a1, unsigned char a2);

struct CTCVillage {
    SubObj* sub;       // +0x00 -> ecx
    int     field4;    // +0x04 -> edx
    // +0x08 ...
    char    pad8[4];   // +0x08..0x0b
    unsigned char b_c; // +0x0c pushed arg
    unsigned char b_d; // +0x0d flag
};

void __fastcall OnInitialActivate(CTCVillage* self)
{
    if (self->b_d) {
        Sub_Call(self->sub, self->field4, self->b_c);
        self->b_d = 0;
    }
}