// CGuiVarTransferBool::TransferToGui @ 0x00a73770

struct GuiTarget;
struct GuiSource;

// callee at 0xa735a0: __fastcall(ecx, edx, bool)
void __fastcall SetGuiBoolValue(GuiTarget *ecxArg, GuiSource *edxArg, bool value);

struct CGuiVarTransferBool
{
    GuiTarget *m_target;   // +0x00 -> ecx of callee
    GuiSource *m_source;   // +0x04 -> edx of callee
    // +0x08 padding
    int        m_pad08;    // +0x08
    bool       m_value;    // +0x0c -> pushed byte
    bool       m_dirty;    // +0x0d -> cleared

    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    SetGuiBoolValue(m_target, m_source, m_value);
    m_dirty = false;
}