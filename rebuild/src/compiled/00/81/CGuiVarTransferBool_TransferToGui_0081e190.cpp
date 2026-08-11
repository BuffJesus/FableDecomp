// CGuiVarTransferBool::TransferToGui  @ 0x0081e190
// __fastcall void TransferToGui(CGuiVarTransferBool* this)
//
// Retail:
//   push esi
//   mov  esi, ecx
//   mov  edx, [esi+4]
//   mov  ecx, [esi]
//   xor  eax, eax
//   mov  al, [esi+0xc]
//   push eax
//   call 0x81e0f0
//   mov  byte [esi+0xd], 0
//   pop  esi
//   ret
//
// The callee takes ecx=[esi] (this) and edx=[esi+4] (register arg), plus a
// stack argument (the zero-extended bool at +0xc). Model it as a fastcall
// helper so the compiler loads ecx and edx from the two members.

struct CGuiVarBoolTarget;

typedef void (__fastcall *SetFn)(CGuiVarBoolTarget* self, void* edxArg, unsigned char val);

struct CGuiVarTransferBool {
    CGuiVarBoolTarget* target;   // +0x00 -> ecx
    void*              arg4;     // +0x04 -> edx
    int                pad8;     // +0x08
    unsigned char      value;    // +0x0c
    unsigned char      dirty;    // +0x0d
};

// Forward callee (fastcall: ecx=self, edx=edxArg, stack: val)
extern void __fastcall CGuiVarBool_Set(CGuiVarBoolTarget* self, void* edxArg, unsigned char val);

void __fastcall CGuiVarTransferBool_TransferToGui(CGuiVarTransferBool* self)
{
    CGuiVarBool_Set(self->target, self->arg4, self->value);
    self->dirty = 0;
}