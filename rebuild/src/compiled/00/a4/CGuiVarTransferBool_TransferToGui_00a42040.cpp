// CGuiVarTransferBool::TransferToGui  @ 0x00a42040
// Layout reconstructed from disasm:
//   [esi+0]   target object pointer (ecx for the call)
//   [esi+4]   handle/id value (edx for the call)
//   [esi+0xc] bool value being transferred (stack arg)
//   [esi+0xd] "changed" flag, cleared after transfer

// The callee at 0xa41e20 receives ecx = obj, edx = handle, and a pushed bool.
// That is a __fastcall(obj, handle, value) signature.
extern void __fastcall GuiVarSetBool(void* obj, int handle, bool value);

struct CGuiVarTransferBool
{
    void* target;   // +0x00
    int   handle;   // +0x04
    int   pad8;     // +0x08
    bool  value;    // +0x0c
    bool  changed;  // +0x0d

    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    GuiVarSetBool(this->target, this->handle, this->value);
    this->changed = false;
}