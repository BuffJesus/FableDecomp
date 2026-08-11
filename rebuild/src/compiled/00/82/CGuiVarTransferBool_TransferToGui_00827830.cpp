// CGuiVarTransferBool::TransferToGui  @ 0x00827830
// __fastcall void TransferToGui(CGuiVarTransferBool* this)

struct GuiTarget;

// callee @ 0x827710 : __fastcall(ecx=target, edx=field, stack=bool value)
void __fastcall GuiTarget_Set(GuiTarget* self, int edx, bool value);

struct CGuiVarTransferBool
{
    GuiTarget* m_pTarget;   // +0x00  -> ecx
    int        m_field;     // +0x04  -> edx
    int        m_pad8;      // +0x08
    bool       m_value;     // +0x0c  -> pushed bool
    unsigned char m_done;   // +0x0d  -> set to 0
    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    GuiTarget_Set(m_pTarget, m_field, m_value);
    m_done = 0;
}