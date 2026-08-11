#include <cstdio>

struct CGuiTarget;

void __fastcall GuiVarStore(CGuiTarget* obj, void* field, bool value);

struct CGuiVarTransferBool
{
    CGuiTarget* m_obj;
    void*       m_field;
    void*       m_pad08;
    bool        m_value;
    bool        m_dirty;

    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    GuiVarStore(this->m_obj, this->m_field, this->m_value);
    this->m_dirty = false;
}

static CGuiTarget* g_obj;
static void*       g_field;
static bool        g_value;
static int         g_calls;

void __fastcall GuiVarStore(CGuiTarget* obj, void* field, bool value)
{
    g_obj = obj;
    g_field = field;
    g_value = value;
    ++g_calls;
}

int main()
{
    int fieldSlot = 0;
    CGuiVarTransferBool v;
    v.m_obj = (CGuiTarget*)0x1234;
    v.m_field = &fieldSlot;
    v.m_pad08 = 0;
    v.m_value = true;
    v.m_dirty = true;

    v.TransferToGui();

    bool ok = (g_calls == 1)
           && (g_obj == (CGuiTarget*)0x1234)
           && (g_field == &fieldSlot)
           && (g_value == true)
           && (v.m_dirty == false);

    // second pass with value=false
    v.m_value = false;
    v.m_dirty = true;
    v.TransferToGui();
    ok = ok && (g_calls == 2) && (g_value == false) && (v.m_dirty == false);

    if (ok) { printf("TRANSFERTOGUI_BOOL_OK\n"); return 0; }
    printf("FAIL calls=%d dirty=%d\n", g_calls, (int)v.m_dirty);
    return 1;
}