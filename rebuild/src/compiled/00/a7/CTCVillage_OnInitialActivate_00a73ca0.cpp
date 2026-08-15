// Byte-exact reconstruction of CTCVillage::OnInitialActivate @ 0x00a73ca0
// __fastcall (this in ecx). Modeled as __fastcall free fn with self in ecx.
//
// Retail body:
//   push esi; mov esi,ecx
//   al = this[0x0d]; if (!al) goto end
//   edx = this[0x04]; ecx = this[0x00]
//   xor eax,eax; al = this[0x0c]   (bool, zero-extended)
//   push eax; call 0xa73ba0        (__fastcall on this[0], edx=this[4], bool arg)
//   this[0x0d] = 0
// end: pop esi; ret
//
// The bool third-arg is what makes VC7.1 emit `xor eax,eax; mov al` (not movzx)
// and schedule the edx/ecx loads ahead of the byte fetch.

struct Callee;

// callee at 0xa73ba0: __fastcall on obj (ecx), edx=second param, one stack bool arg.
void __fastcall Callee_method(Callee* obj, void* edxArg, bool flag);

struct CTCVillage {
    Callee*       field0;  // +0x00  -> ecx to callee
    void*         field4;  // +0x04  -> edx to callee
    int           dummy8;  // +0x08
    bool          fieldC;  // +0x0c  -> pushed bool arg
    unsigned char fieldD;  // +0x0d  -> guard flag
};

void __fastcall OnInitialActivate(CTCVillage* self)
{
    if (self->fieldD) {
        Callee_method(self->field0, self->field4, self->fieldC);
        self->fieldD = 0;
    }
}