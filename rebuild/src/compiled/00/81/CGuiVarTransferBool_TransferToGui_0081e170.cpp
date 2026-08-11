// CGuiVarTransferBool::TransferToGui  @ 0x0081e170
// __fastcall void TransferToGui(CGuiVarTransferBool* this)

struct CGuiTarget;

// Callee at 0x0081e090: __fastcall(ecx=obj, edx=field, stackarg=bool)
void __fastcall GuiVarStore(CGuiTarget* obj, void* field, bool value);

struct CGuiVarTransferBool
{
    CGuiTarget* m_obj;   // +0x00
    void*       m_field; // +0x04
    void*       m_pad08; // +0x08
    bool        m_value; // +0x0c
    bool        m_dirty; // +0x0d

    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    GuiVarStore(this->m_obj, this->m_field, this->m_value);
    this->m_dirty = false;
}