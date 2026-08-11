// CGuiVarTransferBool::TransferToGui @ 009b8970

struct GuiTarget;

// helper at 0x9b84c0: __fastcall on GuiTarget*, with an extra edx arg and a stack byte arg.
// Model as __fastcall: ecx=self, edx=idx, stack=val.
void __fastcall GuiTargetSet(GuiTarget* self, int idx, unsigned char val);

struct CGuiVarTransferBool
{
    GuiTarget* target;   // +0
    int        idx;      // +4
    int        pad8;     // +8
    unsigned char value; // +0xc
    unsigned char flag;  // +0xd

    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    GuiTargetSet(this->target, this->idx, this->value);
    this->flag = 0;
}