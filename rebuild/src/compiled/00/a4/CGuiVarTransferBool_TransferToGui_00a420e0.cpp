// CGuiVarTransferBool::TransferToGui  @ 0x00a420e0
// Faithful VC7.1 reconstruction.

struct GuiTarget;

// callee at 0x00a42060: __fastcall(ecx=obj, edx=idx, stack=bool)
void __fastcall SetGuiValue(GuiTarget *obj, int idx, unsigned char val);

struct CGuiVarTransferBool
{
    GuiTarget *m_target;   // +0x00
    int        m_index;    // +0x04
    // +0x08 padding/other
    int        m_pad8;     // +0x08
    unsigned char m_value; // +0x0c
    unsigned char m_done;  // +0x0d

    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    SetGuiValue(m_target, m_index, m_value);
    m_done = 0;
}