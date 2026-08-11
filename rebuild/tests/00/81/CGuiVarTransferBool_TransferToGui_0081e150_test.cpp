#include <stdio.h>

struct GuiObjA;
struct GuiObjB;

void __fastcall CGuiVarTransferBool_helper(GuiObjA* self, GuiObjB* other, unsigned char val);

struct CGuiVarTransferBool
{
	GuiObjA*      m_pSource;   // +0x00
	GuiObjB*      m_pTarget;   // +0x04
	char          pad8[4];     // +0x08
	signed char   m_value;     // +0x0c
	unsigned char m_dirty;     // +0x0d
	void TransferToGui();
};

// capture what the helper received
static GuiObjA* g_self;
static GuiObjB* g_other;
static unsigned int g_val;
static int      g_calls;

void __fastcall CGuiVarTransferBool_helper(GuiObjA* self, GuiObjB* other, unsigned char val)
{
	g_self = self;
	g_other = other;
	g_val = (unsigned int)val;
	g_calls++;
}

void CGuiVarTransferBool::TransferToGui()
{
	CGuiVarTransferBool_helper(m_pSource, m_pTarget, (unsigned char)m_value);
	m_dirty = 0;
}

int main()
{
	CGuiVarTransferBool o;
	o.m_pSource = (GuiObjA*)0x11112222;
	o.m_pTarget = (GuiObjB*)0x33334444;
	o.m_value = (signed char)0xC3;   // high-bit set: verify zero-extend, not sign-extend
	o.m_dirty = 0x77;

	o.TransferToGui();

	int ok = 1;
	if (g_calls != 1) ok = 0;
	if (g_self != (GuiObjA*)0x11112222) ok = 0;
	if (g_other != (GuiObjB*)0x33334444) ok = 0;
	if (g_val != 0xC3u) ok = 0;         // zero-extended (0xC3, not 0xFFFFFFC3)
	if (o.m_dirty != 0) ok = 0;         // dirty cleared

	printf(ok ? "TRANSFERTOGUI_OK\n" : "TRANSFERTOGUI_FAIL v=%u dirty=%d calls=%d\n",
		g_val, o.m_dirty, g_calls);
	return ok ? 0 : 1;
}