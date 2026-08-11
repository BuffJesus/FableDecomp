#include <cstdio>

struct CGuiVar;
struct CGuiVarSrc;

void __fastcall GuiVarBool_Apply(CGuiVar* target, CGuiVarSrc* source, bool value);

struct CGuiVarTransferBool
{
    CGuiVar*    m_target;
    CGuiVarSrc* m_source;
    int         m_pad8;
    bool        m_value;
    bool        m_dirty;

    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    GuiVarBool_Apply(this->m_target, this->m_source, this->m_value);
    this->m_dirty = false;
}

// Test harness capture
static CGuiVar*    g_target;
static CGuiVarSrc* g_source;
static int         g_value = -1;
static int         g_calls = 0;

void __fastcall GuiVarBool_Apply(CGuiVar* target, CGuiVarSrc* source, bool value)
{
    g_target = target;
    g_source = source;
    g_value  = value ? 1 : 0;
    ++g_calls;
}

int main()
{
    CGuiVarTransferBool obj;
    obj.m_target = (CGuiVar*)0x11112222;
    obj.m_source = (CGuiVarSrc*)0x33334444;
    obj.m_pad8   = 0;
    obj.m_value  = true;
    obj.m_dirty  = true;   // must be cleared

    obj.TransferToGui();

    bool ok = true;
    if (g_calls != 1) ok = false;
    if (g_target != (CGuiVar*)0x11112222) ok = false;
    if (g_source != (CGuiVarSrc*)0x33334444) ok = false;
    if (g_value != 1) ok = false;
    if (obj.m_dirty != false) ok = false;

    // second case: value false
    obj.m_value = false;
    obj.m_dirty = true;
    obj.TransferToGui();
    if (g_value != 0) ok = false;
    if (obj.m_dirty != false) ok = false;

    printf(ok ? "TRANSFERBOOL_OK\n" : "TRANSFERBOOL_FAIL\n");
    return ok ? 0 : 1;
}