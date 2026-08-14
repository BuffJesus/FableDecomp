void __fastcall Sub_9b84c0(void* ecx, void* edx, unsigned char arg);

struct CTCVillage {
    void*         field_0;   // +0x00  -> ecx
    void*         field_4;   // +0x04  -> edx
    char          pad8[4];   // +0x08
    unsigned char field_C;   // +0x0c
    unsigned char field_D;   // +0x0d
};

void __fastcall OnInitialActivate(CTCVillage* self)
{
    if (self->field_D) {
        Sub_9b84c0(self->field_0, self->field_4, self->field_C);
        self->field_D = 0;
    }
}