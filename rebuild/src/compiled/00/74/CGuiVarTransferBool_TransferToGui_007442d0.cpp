// CGuiVarTransferBool::TransferToGui @ 007442d0

struct CGuiVarTarget;

// helper @ 0x7441d0 : __fastcall(target*, int field, bool value)
void __fastcall CGuiVarTransferBool_helper(CGuiVarTarget* target, int field, bool value);

struct CGuiVarTransferBool {
    CGuiVarTarget* target; // +0
    int            field;  // +4
    // +8 padding/other
    int            pad8;   // +8
    bool           value;  // +0xc
    unsigned char  dirty;  // +0xd
};

void __fastcall CGuiVarTransferBool_TransferToGui(CGuiVarTransferBool* self)
{
    CGuiVarTransferBool_helper(self->target, self->field, self->value);
    self->dirty = 0;
}