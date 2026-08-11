// CGuiVarTransferBool::TransferToGui @ 004bbbc0
struct CGuiVarTransferBool {
    void* f0;    // +0x00
    void* f4;    // +0x04
    char  pad8[4]; // +0x08
    bool  f0c;   // +0x0c
    bool  f0d;   // +0x0d
};

// helper @ 0x4bbb40: __fastcall(ecx, edx, bool b)
extern void __fastcall helper_4bbb40(void* ecx, void* edx, bool b);

void __fastcall TransferToGui(CGuiVarTransferBool* self)
{
    helper_4bbb40(self->f0, self->f4, self->f0c);
    self->f0d = false;
}