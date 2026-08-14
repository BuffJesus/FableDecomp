struct CTCVillage {
    void* field0;      // +0x00
    void* field4;      // +0x04
    char  pad8[4];     // +0x08
    unsigned char b0c; // +0x0c
    unsigned char b0d; // +0x0d
};

// callee at 0x9b9490: __fastcall(ecx=[esi], edx=[esi+4], stack u8 arg)
extern void __fastcall Callee(void* self, void* a1, unsigned char a2);

void __fastcall OnInitialActivate(CTCVillage* self)
{
    if (self->b0d) {
        Callee(self->field0, self->field4, self->b0c);
        self->b0d = 0;
    }
}