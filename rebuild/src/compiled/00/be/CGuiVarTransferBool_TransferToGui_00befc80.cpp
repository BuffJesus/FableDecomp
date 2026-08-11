// CGuiVarTransferBool::TransferToGui  (retail 0x00befc80)
// __fastcall void TransferToGui(CGuiVarTransferBool* this)
//
// esi=this
//   edx = [this+4]
//   ecx = [this+0]        -> callee's 'this' (fastcall arg1)
//   al  = (unsigned char)[this+0xc], pushed as stack arg
//   call 0x00befc20  (fastcall: arg1=ecx=[this+0], arg2=edx=[this+4], stack byte)
//   [this+0xd] = 0
//   ret

struct CGuiVarBoolTarget;

// callee at 0x00befc20: fastcall member-style helper taking (self, edx-arg, byte)
void __fastcall CGuiVarBool_Apply(CGuiVarBoolTarget* self, int arg2, unsigned char val);

struct CGuiVarTransferBool
{
    CGuiVarBoolTarget* m_pTarget;  // +0x00
    int                m_arg;      // +0x04
    int                m_pad08;    // +0x08
    unsigned char      m_value;    // +0x0c
    unsigned char      m_dirty;    // +0x0d

    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    CGuiVarBool_Apply(this->m_pTarget, this->m_arg, this->m_value);
    this->m_dirty = 0;
}