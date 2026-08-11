// CGuiVarTransferBool::TransferToGui  @ 009e7ec0
// __fastcall void TransferToGui(CGuiVarTransferBool* this)

struct GuiVarTarget;

// callee @ 0x9e7e40: __fastcall (ecx=p1, edx=p2, stack=byte value)
extern void __fastcall TransferBoolHelper(void* p1, void* p2, bool val);

struct CGuiVarTransferBool
{
    void*          m_pTarget;      // +0x00 -> ecx (this for helper)
    void*          m_pVar;         // +0x04 -> edx
    unsigned char  m_pad08[4];     // +0x08
    bool           m_value;        // +0x0c
    bool           m_dirty;        // +0x0d

    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    TransferBoolHelper(m_pTarget, m_pVar, m_value);
    m_dirty = false;
}