// Byte-exact reconstruction of CTCVillage::OnInitialActivate @ 0x00a84b60
// __fastcall(void) -> modeled as __fastcall free fn with self in ecx.

struct Inner;

struct CTCVillage {
    Inner *field0;   // +0x00
    int    field4;   // +0x04
    char   pad8[4];  // +0x08
    char   fieldC;   // +0x0C
    char   fieldD;   // +0x0D  (bool guard flag)
};

// The direct call target: a __fastcall helper taking (Inner* self, int arg).
// Modeled __fastcall: ecx=self, edx=(loaded from [esi+4]), stack arg = fieldC.
// unsigned char param makes VC7.1 emit the retail xor eax,eax; mov al,[esi+0xc] widen.
extern void __fastcall Helper(Inner *self, int edxVal, unsigned char arg);

void __fastcall OnInitialActivate(CTCVillage *self)
{
    if (self->fieldD) {
        Helper(self->field0, self->field4, (unsigned char)self->fieldC);
        self->fieldD = 0;
    }
}