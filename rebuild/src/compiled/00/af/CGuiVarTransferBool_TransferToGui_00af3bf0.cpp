// CGuiVarTransferBool::TransferToGui  @ 00af3bf0

struct GuiTarget;

// callee at 0xaf3b70: __fastcall (ecx=target, edx=id, stack bool)
void __fastcall GuiBoolStore(GuiTarget* self, int id, bool value);

struct CGuiVarTransferBool
{
    GuiTarget*    m_target;   // +0x00
    int           m_id;       // +0x04
    int           m_pad8;     // +0x08
    bool          m_value;    // +0x0c
    bool          m_dirty;    // +0x0d

    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    GuiBoolStore(this->m_target, this->m_id, this->m_value);
    this->m_dirty = false;
}