struct CGuiVarTransferBool {
    void* target;        // +0x00
    void* context;       // +0x04
    char  pad8[4];       // +0x08
    bool  value;         // +0x0C
    unsigned char done;  // +0x0D
};

// helper at 0x4d2470: __fastcall(ecx=a, edx=b, stack=bool c)
extern "C" void __fastcall Helper_4d2470(void* a, void* b, bool c);

void __fastcall TransferToGui(CGuiVarTransferBool* self)
{
    Helper_4d2470(self->target, self->context, self->value);
    self->done = 0;
}