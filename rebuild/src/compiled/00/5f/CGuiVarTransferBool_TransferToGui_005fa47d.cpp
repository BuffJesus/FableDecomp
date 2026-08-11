// CGuiVarTransferBool::TransferToGui @ 005fa47d

struct GuiTarget;

// helper @ 0x5fa3dd: this=ecx, edx arg, one stack bool arg
void __fastcall Helper_5fa3dd(GuiTarget* self, int /*edx*/ edxArg, bool boolArg);

struct CGuiVarTransferBool {
    GuiTarget* target;   // +0x00  -> ecx
    int        field4;   // +0x04  -> edx
    char       pad8[4];  // +0x08
    bool       value;    // +0x0C  -> al (bool byte)
    char       flag;     // +0x0D  -> set to 0
};

void __fastcall TransferToGui(CGuiVarTransferBool* self)
{
    Helper_5fa3dd(self->target, self->field4, self->value);
    self->flag = 0;
}