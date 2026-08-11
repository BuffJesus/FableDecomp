// CGuiVarTransferBool::TransferToGui  @ 0x00a73c40  __fastcall(void)
// Callee at 0x00a73b20 is __fastcall(ecx, edx, stack-arg): setter that takes
// the target object (ecx), a secondary value (edx) and the bool value (stack).
extern "C" void __fastcall Transfer_a73b20(void* self, int edxarg, bool val);

struct CGuiVarTransferBool
{
    void*         m_target;   // +0x00 -> ecx for the callee
    int           m_edxval;   // +0x04 -> edx for the callee
    int           m_pad08;    // +0x08
    bool          m_value;    // +0x0c -> the bool pushed to the callee
    bool          m_written;  // +0x0d -> cleared after the transfer

    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    Transfer_a73b20(m_target, m_edxval, m_value);
    m_written = false;
}