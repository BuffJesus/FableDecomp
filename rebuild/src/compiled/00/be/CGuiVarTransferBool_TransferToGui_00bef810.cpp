// CGuiVarTransferBool::TransferToGui  @ 0x00bef810
// __fastcall void TransferToGui(CGuiVarTransferBool* this)

struct CGuiTarget;

// callee @ 0xbef5c0 : ecx=self(+0), edx=(+4), stack=byte(+0xc)
void __fastcall CGuiTarget_Set(CGuiTarget* self, int edx, bool value);

struct CGuiVarTransferBool {
    CGuiTarget* target;   // +0x00
    int         param;    // +0x04
    char        pad8[4];  // +0x08
    bool        curValue; // +0x0c
    bool        dirty;    // +0x0d
};

void __fastcall CGuiVarTransferBool_TransferToGui(CGuiVarTransferBool* self)
{
    CGuiTarget_Set(self->target, self->param, self->curValue);
    self->dirty = false;
}