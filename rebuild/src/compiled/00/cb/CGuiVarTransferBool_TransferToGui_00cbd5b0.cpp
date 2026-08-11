// CGuiVarTransferBool::TransferToGui @ 0x00cbd5b0
// __fastcall void TransferToGui(CGuiVarTransferBool* this)

// Callee at 0xcbd370: ecx=[this+0], edx=[this+4], stack bool arg.
// Modeled as a __fastcall free function (ecx=self, edx=arg2, stack=value).
void __fastcall GuiVarBool_Apply(void* self, int arg2, bool value);

struct CGuiVarTransferBool
{
    void*         pTarget;   // +0x00  -> ecx of callee
    int           arg2;      // +0x04  -> edx of callee
    int           pad8;      // +0x08
    bool          value;     // +0x0c  -> pushed (xor eax; mov al)
    unsigned char dirty;     // +0x0d  -> cleared to 0

    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    GuiVarBool_Apply(this->pTarget, this->arg2, this->value);
    this->dirty = 0;
}