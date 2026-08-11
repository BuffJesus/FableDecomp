// CGuiVarTransferBool::TransferToGui @ 0077f4c0
struct CGuiVarTransferBool {
    void* pGui;    // +0  -> ecx
    void* pVar;    // +4  -> edx
    int   pad8;    // +8
    unsigned char curValue; // +0xc
    unsigned char dirty;    // +0xd
};

// helper @ 0x77f440 : __fastcall(ecx, edx, stack int), cleans its own arg
extern void __fastcall GuiVarBoolHelper(void* pGui, void* pVar, unsigned char value);

void __fastcall CGuiVarTransferBool_TransferToGui(CGuiVarTransferBool* self)
{
    GuiVarBoolHelper(self->pGui, self->pVar, self->curValue);
    self->dirty = 0;
}