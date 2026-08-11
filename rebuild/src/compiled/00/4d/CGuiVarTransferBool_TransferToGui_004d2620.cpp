// CGuiVarTransferBool::TransferToGui @ 004d2620
// helper @ 0x4d24f0: ecx=f0, edx=f4, stack=bool
extern void __fastcall helper_4d24f0(void* ecx, void* edx, bool b);

struct CGuiVarTransferBool {
    void* f0;          // +0x00
    void* f4;          // +0x04
    char  pad8[4];     // +0x08
    bool  f0c;         // +0x0c
    unsigned char f0d; // +0x0d

    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    helper_4d24f0(this->f0, this->f4, this->f0c);
    this->f0d = 0;
}