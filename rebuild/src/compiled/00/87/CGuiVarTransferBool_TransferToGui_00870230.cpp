// CGuiVarTransferBool::TransferToGui  (retail 0x00870230)

struct CGuiTarget;

// callee at 0x8701d0: __fastcall(this=ecx, arg2=edx, bool byte on stack)
void __fastcall GuiSetBool(CGuiTarget* self, void* edx, unsigned char v);

struct CGuiVarTransferBool
{
    CGuiTarget* target;   // +0x00
    void*       cookie;   // +0x04
    int         pad08;    // +0x08
    unsigned char value;  // +0x0c
    unsigned char dirty;  // +0x0d

    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    GuiSetBool(this->target, this->cookie, this->value);
    this->dirty = 0;
}