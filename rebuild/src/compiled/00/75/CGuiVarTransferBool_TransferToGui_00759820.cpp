// CGuiVarTransferBool::TransferToGui @ 0x00759820
struct CGuiVarTransferBool {
    void*         gui;    // +0x00 -> ecx
    int           edxv;   // +0x04 -> edx
    char          pad8;   // +0x08
    char          pad9;   // +0x09
    char          pada;   // +0x0a
    char          padb;   // +0x0b
    unsigned char value;  // +0x0c
    unsigned char dirty;  // +0x0d
};

// helper @ 0x7597c0 : this in ecx, second reg arg in edx, one byte stack arg
extern void __fastcall Helper_7597C0(void* self, int edx, unsigned char arg);

void __fastcall TransferToGui(CGuiVarTransferBool* self)
{
    Helper_7597C0(self->gui, self->edxv, self->value);
    self->dirty = 0;
}