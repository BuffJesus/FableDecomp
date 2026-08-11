// CTCVillage::ResetCrier  @ 0x00c1bb60  __fastcall(float)
// Faithful reconstruction: store (char) of a scaled float into field at +1.
// The (long) cast forces VC7.1 to emit the __ftol helper call (the disasm's call 0xbfea70),
// with the float argument left on the x87 stack (fld;fmul;call;mov [esi+1],al;ret 4).

// global scale constant referenced by fmul dword ptr [0x1230a08]
static const float k_CrierScale = 0.5f;

struct CTCVillage
{
    char pad0;        // +0
    char crier;       // +1  <- byte ptr [esi + 1]
};

void __fastcall CTCVillage_ResetCrier(CTCVillage* self, int /*edx*/, float x)
{
    self->crier = (char)(long)(x * k_CrierScale);
}