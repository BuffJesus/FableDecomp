// CGuiVarTransferBool::TransferToGui  @ 0x0081e150
// __fastcall void TransferToGui(CGuiVarTransferBool* this)
//
// Disasm:
//   push esi
//   mov  esi, ecx
//   mov  edx, [esi+4]
//   mov  ecx, [esi]
//   xor  eax, eax
//   mov  al,  [esi+0xc]
//   push eax
//   call 0x81e030
//   mov  byte [esi+0xd], 0
//   pop  esi
//   ret
//
// The callee at 0x81e030 takes this=[esi] in ecx, a second pointer=[esi+4]
// in edx, and a zero-extended value from [esi+0xc] on the stack.  Modeling it
// as __fastcall(ecx, edx, stack) with an unsigned char param reproduces the
// retail xor eax,eax / mov al / push eax widening idiom exactly.

struct GuiObjA;
struct GuiObjB;

// helper @ 0x81e030 : __fastcall(A* self [ecx], B* other [edx], unsigned char val [stack])
void __fastcall CGuiVarTransferBool_helper(GuiObjA* self, GuiObjB* other, unsigned char val);

struct CGuiVarTransferBool
{
	GuiObjA*      m_pSource;   // +0x00 -> ecx
	GuiObjB*      m_pTarget;   // +0x04 -> edx
	char          pad8[4];     // +0x08
	signed char   m_value;     // +0x0c
	unsigned char m_dirty;     // +0x0d

	void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
	CGuiVarTransferBool_helper(m_pSource, m_pTarget, m_value);
	m_dirty = 0;
}