// CGuiVarTransferBool::TransferToGui  @ 0x0088d1b0
// Retail disasm:
//   push esi; mov esi,ecx
//   mov edx,[esi+4]; mov ecx,[esi]
//   xor eax,eax; mov al,[esi+0xc]; push eax
//   call 0x88d130
//   mov byte [esi+0xd],0
//   pop esi; ret

// The callee takes ecx=[esi], edx=[esi+4], and one stack arg (a bool).
// Model it as a __fastcall(ecx, edx, stackarg).
extern "C" void __fastcall CGuiVarTransferBool_Setter(void* pThis, void* edxVal, unsigned char value);

struct CGuiVarTransferBool
{
    void*         field_0;   // +0x00  -> ecx for callee
    void*         field_4;   // +0x04  -> edx for callee
    int           field_8;   // +0x08  (padding/other)
    unsigned char field_C;   // +0x0C  bool value read
    unsigned char field_D;   // +0x0D  byte cleared to 0

    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    CGuiVarTransferBool_Setter(this->field_0, this->field_4, this->field_C);
    this->field_D = 0;
}