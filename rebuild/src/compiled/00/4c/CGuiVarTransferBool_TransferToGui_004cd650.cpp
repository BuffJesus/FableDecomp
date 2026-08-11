// CGuiVarTransferBool::TransferToGui @ 004cd650
struct CGuiVarTransferBool {
    void*         f0;   // +0x00  -> helper this (ecx)
    int           f4;   // +0x04  -> helper edx arg
    int           f8;   // +0x08
    bool          fC;   // +0x0c  bool value (stack arg)
    bool          fD;   // +0x0d  cleared after
};

// helper @ 0x4cd5d0 : __fastcall-style, ecx=f0, edx=f4, one stack bool arg
extern void __fastcall CGuiVarTransferBool_Helper(void* self, int edx, bool value);

void __fastcall CGuiVarTransferBool_TransferToGui(CGuiVarTransferBool* self)
{
    CGuiVarTransferBool_Helper(self->f0, self->f4, self->fC);
    self->fD = false;
}