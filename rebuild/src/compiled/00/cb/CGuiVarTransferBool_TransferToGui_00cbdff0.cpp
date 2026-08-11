// CGuiVarTransferBool::TransferToGui @ 00cbdff0

struct GuiTargetObj;

// callee @ 0xcbdf70 : ecx=obj, edx=second reg arg, one pushed stack arg (bool)
void __fastcall SetGuiBoolValue(GuiTargetObj* obj, void* edxArg, unsigned char value);

struct CGuiVarTransferBool
{
    GuiTargetObj* pTarget;   // +0x00
    void*         edxArg;    // +0x04
    int           pad08;     // +0x08
    unsigned char curValue;  // +0x0C
    unsigned char dirtyFlag; // +0x0D

    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    SetGuiBoolValue(this->pTarget, this->edxArg, this->curValue);
    this->dirtyFlag = 0;
}