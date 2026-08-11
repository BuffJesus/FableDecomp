// CGuiVarTransferBool::TransferToGui @ 005e3cb0
// helper(ecx=[esi], edx=[esi+4], (u8)this->f0c); this->f0d=0

struct CGuiVarTransferBool {
    void* f0;   // +0x00
    void* f4;   // +0x04
    char  pad8[4]; // +0x08
    unsigned char f0c; // +0x0c
    unsigned char f0d; // +0x0d
};

// helper @ 0x5e3ad0: __fastcall(ecx, edx, stack u8)
extern "C" void __fastcall gui_bool_helper(void* ecx, void* edx, unsigned char v);

void __fastcall TransferToGui(CGuiVarTransferBool* self)
{
    gui_bool_helper(self->f0, self->f4, self->f0c);
    self->f0d = 0;
}