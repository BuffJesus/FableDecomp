// CGuiVarTransferBool::TransferToGui  @ 0x00a84ff0

// Forward-declared external helper (0x00a84cf0). It is invoked with
// ecx = self->m_pTarget, edx = self->m_data, and a pushed byte value.
// Model it as an __fastcall taking (ecx, edx) plus one stack int.
void __fastcall CGuiVarTransferBool_Apply(void *pTarget, int edxData, bool value);

struct CGuiVarTransferBool
{
    void *m_pTarget;    // +0x00  -> ecx
    int   m_data;       // +0x04  -> edx
    // +0x08 (unused here)
    int   m_pad08;
    bool m_value;            // +0x0c  pushed as int
    unsigned char m_dirty;   // +0x0d  cleared to 0

    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    CGuiVarTransferBool_Apply(m_pTarget, m_data, m_value);
    m_dirty = 0;
}