// CTCVillage::OnInitialActivate @ 0x0086e070
// __fastcall void(CTCVillage*)

struct SomeTarget;

// callee at 0x86dd20: takes ecx, edx, and one stack arg (int)
// __fastcall passes arg1->ecx, arg2->edx, remaining on stack
extern void __fastcall Callee_86dd20(void* ecxArg, void* edxArg, unsigned char stackArg);

struct CTCVillage {
    void*        field_0;   // +0x00  -> ecx target
    void*        field_4;   // +0x04  -> edx arg
    // +0x08 padding
    char         pad8[4];
    unsigned char field_C;  // +0x0C
    unsigned char field_D;  // +0x0D  flag
};

void __fastcall OnInitialActivate(CTCVillage* self)
{
    if (self->field_D) {
        Callee_86dd20(self->field_0, self->field_4, self->field_C);
        self->field_D = 0;
    }
}