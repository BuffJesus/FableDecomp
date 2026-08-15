// Byte-exact reconstruction of CTCVillage::OnInitialActivate @ 0x00cbe010

struct Sub;

// callee @ 0xcbdef0: __fastcall(this=ecx, edx arg, stack byte arg)
extern "C" void __fastcall Sub_notify(Sub* self, int arg, unsigned char flag);

struct CTCVillage {
    Sub*          field0;   // +0x00
    int           field4;   // +0x04
    // +0x08 padding
    char          pad8[4];  // +0x08
    unsigned char byte_0c;  // +0x0c
    unsigned char byte_0d;  // +0x0d
};

void __fastcall CTCVillage_OnInitialActivate(CTCVillage* self)
{
    if (self->byte_0d) {
        Sub_notify(self->field0, self->field4, self->byte_0c);
        self->byte_0d = 0;
    }
}