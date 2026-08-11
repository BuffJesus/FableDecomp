// CGuiVarTransferBool::TransferToGui  @ 0x00993a10
// __fastcall void (CGuiVarTransferBool* this)
//
// disasm:
//   push esi ; mov esi,ecx
//   mov edx,[esi+4]      ; second fastcall reg arg
//   mov ecx,[esi]        ; target object (this for the call)
//   xor eax,eax ; mov al,[esi+0xc] ; zero-extend bool
//   push eax            ; stack arg (bool)
//   call 0x9939b0       ; ((Target*)m_target)->Set(m_field, bool) [fastcall shape]
//   mov byte [esi+0xd],0
//   pop esi ; ret

// Callee: ecx=self, edx=arg2, one stack arg (bool) -> free __fastcall.
void __fastcall GuiSetBoolTarget(void* self, int arg2, bool value);

struct CGuiVarTransferBool
{
    void* m_target;        // +0x00
    int   m_field;         // +0x04
    int   m_pad8;          // +0x08
    bool  m_value;         // +0x0c
    bool  m_dirty;         // +0x0d

    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    GuiSetBoolTarget(this->m_target, this->m_field, this->m_value);
    this->m_dirty = 0;
}