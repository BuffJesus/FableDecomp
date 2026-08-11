// CGuiVarTransferBool::TransferToGui  @ 0086de70

// The callee at 0x86dd20 receives ecx and edx as its first two args
// (fastcall), plus one stack byte. Model it as a free __fastcall helper.
extern void __fastcall GuiTransferBoolHelper(void *a, void *b, unsigned char v);

struct CGuiVarTransferBool
{
    void         *m_pTarget;   // +0x00  -> ecx of callee
    void         *m_pSource;   // +0x04  -> edx of callee
    void         *m_pad08;     // +0x08
    unsigned char m_value;     // +0x0c
    unsigned char m_dirty;     // +0x0d

    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    GuiTransferBoolHelper(m_pTarget, m_pSource, m_value);
    m_dirty = 0;
}