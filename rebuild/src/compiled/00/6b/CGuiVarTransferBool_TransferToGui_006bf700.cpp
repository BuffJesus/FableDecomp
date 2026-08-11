// CGuiVarTransferBool::TransferToGui @ 0x006bf700

struct CGuiVarTransferBool
{
    void*         field0;   // +0x00 -> ecx to helper
    void*         field4;   // +0x04 -> edx to helper
    int           field8;   // +0x08
    bool          valueC;   // +0x0c bool value pushed
    unsigned char valueD;   // +0x0d cleared to 0
};

// helper @ 0x6bf6a0, __fastcall(ecx=field0, edx=field4, stack: bool value)
extern void __fastcall guivar_bool_helper(void* self, void* edx, bool value);

void __fastcall TransferToGui(CGuiVarTransferBool* self)
{
    guivar_bool_helper(self->field0, self->field4, self->valueC);
    self->valueD = 0;
}