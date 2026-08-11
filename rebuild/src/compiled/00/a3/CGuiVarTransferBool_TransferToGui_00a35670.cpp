// CGuiVarTransferBool::TransferToGui  @ 0x00a35670  (__fastcall)

struct CGuiVarTransferBool
{
    void*         field_0;   // +0x00  (ecx to callee)
    int           field_4;   // +0x04  (edx to callee)
    int           pad_8;     // +0x08
    unsigned char field_C;   // +0x0c  (pushed arg)
    unsigned char field_D;   // +0x0d  (cleared)

    void TransferToGui();
};

// callee at 0x00a355f0: __fastcall taking (ecx, edx, byte-on-stack)
extern void __fastcall CGuiVarTransferBool_Emit(void* a, int b, unsigned char c);

void CGuiVarTransferBool::TransferToGui()
{
    CGuiVarTransferBool_Emit(this->field_0, this->field_4, this->field_C);
    this->field_D = 0;
}