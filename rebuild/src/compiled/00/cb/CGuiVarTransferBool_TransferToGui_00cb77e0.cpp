// CGuiVarTransferBool::TransferToGui  @ 0x00cb77e0
// Faithful VC7.1 reconstruction.

struct GuiVarTarget;

// External helper at 0x00cb7780. It is invoked as a __fastcall taking the
// target pointer in ecx, an int in edx, and a bool pushed on the stack.
void __fastcall GuiVarBoolSet(GuiVarTarget* target, int index, bool value);

struct CGuiVarTransferBool
{
    GuiVarTarget* m_target;   // +0x00 -> ecx
    int           m_index;    // +0x04 -> edx
    // +0x08 padding / other
    int           m_pad08;    // +0x08
    bool          m_value;    // +0x0c
    bool          m_dirty;    // +0x0d

    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    GuiVarBoolSet(this->m_target, this->m_index, this->m_value);
    this->m_dirty = false;
}