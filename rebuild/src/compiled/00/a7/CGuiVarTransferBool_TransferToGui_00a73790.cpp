// CGuiVarTransferBool::TransferToGui  @ 00a73790
struct GuiTarget;

// callee 0xa73600: ecx=GuiTarget*, edx=int, stack=bool
void __fastcall GuiTarget_Set(GuiTarget* self, int edx, bool value);

struct CGuiVarTransferBool {
    GuiTarget* target;   // +0
    int        edxArg;   // +4
    char       pad8[4];  // +8
    bool       value;    // +0xc
    bool       dirty;    // +0xd

    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui() {
    GuiTarget_Set(this->target, this->edxArg, this->value);
    this->dirty = false;
}