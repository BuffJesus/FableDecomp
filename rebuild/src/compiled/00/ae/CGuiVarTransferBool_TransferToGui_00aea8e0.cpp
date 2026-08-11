// CGuiVarTransferBool::TransferToGui  @ 0x00aea8e0
// Faithful VC7.1 reconstruction.

struct CGuiVarTransferBool
{
    void* m_pTarget;   // +0x00  (ecx to callee)
    int   m_edxArg;    // +0x04  (edx to callee)
    // +0x08 unused here
    int   m_pad08;
    bool  m_value;   // +0x0c  bool pushed as arg
    unsigned char m_dirty;   // +0x0d  cleared after transfer
    void TransferToGui();
};

// The callee at 0xaea880: __fastcall (ecx = target, edx = extra), plus a stack bool arg.
extern void __fastcall CGuiVarTransferBool_Apply(void* pTarget, int edxArg, bool value);

void CGuiVarTransferBool::TransferToGui()
{
    CGuiVarTransferBool_Apply(this->m_pTarget, this->m_edxArg, this->m_value);
    this->m_dirty = 0;
}