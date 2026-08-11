// CGuiVarTransferBool::TransferToGui  @ 0086de90
// Retail disasm:
//   push esi; mov esi,ecx
//   mov edx,[esi+4]; mov ecx,[esi]
//   xor eax,eax; mov al,[esi+0xc]; push eax
//   call 0x86dd80
//   mov byte [esi+0xd],0
//   pop esi; ret
//
// The callee at 0x86dd80 receives this=[esi] in ecx, [esi+4] in edx and the
// zero-extended byte [esi+0xc] as a stack argument -> it is a __fastcall
// helper taking (target, value, bool).

extern void __fastcall CGuiVarTransferBool_setter(int target, int value, unsigned char flag);

struct CGuiVarTransferBool
{
    int           m_target;   // +0x00 -> ecx
    int           m_value;    // +0x04 -> edx
    int           m_pad08;    // +0x08
    unsigned char m_flag;     // +0x0c -> pushed byte
    unsigned char m_dirty;    // +0x0d -> cleared

    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    CGuiVarTransferBool_setter(this->m_target, this->m_value, this->m_flag);
    this->m_dirty = 0;
}