// CGuiVarTransferBool::TransferToGui  @ 009b94f0

// The call at 0x9b9490 receives ecx and edx explicitly set plus one stack arg:
//   ecx = self->m_pFirst, edx = self->m_pSecond, stack = (unsigned char)self->m_value
// That is a __fastcall free function with two register params and a stack param.
void __fastcall GuiVarSetBool(void* pFirst, int pSecond, unsigned char v);

struct CGuiVarTransferBool
{
    void*         m_pFirst;   // +0x00
    int           m_pSecond;  // +0x04
    // +0x08 padding/other
    int           m_pad08;    // +0x08
    unsigned char m_value;    // +0x0c
    unsigned char m_flag;     // +0x0d

    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    GuiVarSetBool(this->m_pFirst, this->m_pSecond, this->m_value);
    this->m_flag = 0;
}