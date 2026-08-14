// CTCVillage::OnInitialActivate reconstruction

struct Target;

// callee at 0x81e030: __fastcall on Target*, takes a byte-sized arg.
// ecx=[esi], edx=[esi+4]. Model as __fastcall(ecx, edx, arg).
void __fastcall Callee(Target* ecxObj, void* edxArg, unsigned char arg);

struct CTCVillage
{
    Target* field0;   // +0 -> ecx
    void*   field4;   // +4 -> edx
    char    pad8;      // +8
    char    pad9;
    char    padA;
    char    padB;
    unsigned char fieldC; // +0xc
    unsigned char fieldD; // +0xd flag
};

void __fastcall OnInitialActivate(CTCVillage* self)
{
    if (self->fieldD)
    {
        Callee(self->field0, self->field4, self->fieldC);
        self->fieldD = 0;
    }
}