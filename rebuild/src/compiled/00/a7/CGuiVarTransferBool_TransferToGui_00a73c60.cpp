// CGuiVarTransferBool::TransferToGui  @ 0x00a73c60

struct GuiTarget {
    // __fastcall member: this=ecx, first arg=edx, second arg on stack
    void __fastcall Set(int a, unsigned char b);
};

struct CGuiVarTransferBool {
    GuiTarget* target;   // +0x00
    int        param;    // +0x04
    char       pad8[4];  // +0x08
    unsigned char value; // +0x0c
    char       flag;     // +0x0d

    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    this->target->Set(this->param, this->value);
    this->flag = 0;
}