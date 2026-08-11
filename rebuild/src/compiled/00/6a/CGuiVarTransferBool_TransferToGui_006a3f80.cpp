// CGuiVarTransferBool::TransferToGui @ 006a3f80
struct CGuiVarTransferBool {
    void* f0;    // +0x00 -> ecx
    void* f4;    // +0x04 -> edx
    unsigned char f8;
    unsigned char f9;
    unsigned char fa;
    unsigned char fb;
    bool f0c;            // +0x0c  value transferred
    bool f0d;            // +0x0d  cleared to 0
};

// helper @ 0x6a3f20: __fastcall(ecx, edx, bool pushed as int)
extern void __fastcall CGuiVarTransferBool_helper(void* ecx, void* edx, bool val);

void __fastcall CGuiVarTransferBool_TransferToGui(CGuiVarTransferBool* self)
{
    CGuiVarTransferBool_helper(self->f0, self->f4, self->f0c);
    self->f0d = false;
}