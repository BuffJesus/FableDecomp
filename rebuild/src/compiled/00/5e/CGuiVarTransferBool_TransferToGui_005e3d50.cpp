// CGuiVarTransferBool::TransferToGui @ 005e3d50

struct GuiTarget;

// helper @ 0x5e3cd0 : __fastcall(GuiTarget* ecx, int edx, bool val)
extern void __fastcall GuiSetBool(GuiTarget* self, int edx, bool val);

struct CGuiVarTransferBool {
    GuiTarget* target;   // +0x0  -> helper ecx
    int        arg2;     // +0x4  -> helper edx
    int        pad8;     // +0x8
    bool       value;    // +0xc  bool arg pushed
    char       flag;     // +0xd  cleared to 0
};

void __fastcall TransferToGui(CGuiVarTransferBool* self)
{
    GuiSetBool(self->target, self->arg2, self->value);
    self->flag = 0;
}