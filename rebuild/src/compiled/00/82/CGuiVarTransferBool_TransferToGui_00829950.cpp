// CGuiVarTransferBool::TransferToGui  @ 0x00829950

// The engine call at 0x8298f0 receives:
//   ecx = this->m_a  (field +0)
//   edx = this->m_b  (field +4)
//   stack arg = this->m_value (byte at +0xc), zero-extended
// This is a __fastcall (2 register args + 1 stack arg).
void __fastcall SetGuiBoolValue(void* dst, int idx, bool value);

struct CGuiVarTransferBool
{
    void* m_a;      // +0
    int   m_b;      // +4
    int   m_pad;    // +8
    bool  m_value;  // +0xc
    bool  m_dirty;  // +0xd

    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    SetGuiBoolValue(m_a, m_b, m_value);
    m_dirty = false;
}