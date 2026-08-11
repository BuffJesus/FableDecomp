// CGuiVarTransferBool::TransferToGui  @ 0x00cbdfd0
// Forwards (f0 -> ecx, f4 -> edx, bool fc -> stack) to a fastcall sink,
// then clears the flag at +0xd.

void __fastcall CGuiVarTransferBool_sink(void *self, int val, bool flag);

struct CGuiVarTransferBool
{
    void *f0;      // +0x00
    int   f4;      // +0x04
    char  pad8[4]; // +0x08
    bool  fc;      // +0x0c  bool value
    bool  fd;      // +0x0d  dirty flag

    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    CGuiVarTransferBool_sink(this->f0, this->f4, this->fc);
    this->fd = false;
}