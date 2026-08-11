// CGuiVarTransferBool::TransferToGui  @ 009b8990
// __fastcall, void(CGuiVarTransferBool*)

// The engine callee is a __fastcall: ecx = target object, edx = a second
// pointer/int, plus one stack byte argument.
void __fastcall GuiVarTransferBoolCommit(void* target, int edx, unsigned char value);

struct CGuiVarTransferBool
{
    void*         m_target;   // +0x00  -> ecx of the callee
    int           m_edxArg;   // +0x04  -> edx of the callee
    int           m_pad8;     // +0x08
    unsigned char m_value;    // +0x0C  bool value to push
    unsigned char m_dirty;    // +0x0D  cleared after commit

    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    GuiVarTransferBoolCommit(this->m_target, this->m_edxArg, this->m_value);
    this->m_dirty = 0;
}