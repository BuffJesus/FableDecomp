#include <cstdio>

struct GuiVarTarget { int tag; };

static GuiVarTarget* g_target;
static int  g_index = -999;
static int  g_value = -999;

void __fastcall GuiVarBoolSet(GuiVarTarget* target, int index, bool value)
{
    g_target = target;
    g_index  = index;
    g_value  = value ? 1 : 0;
}

struct CGuiVarTransferBool
{
    GuiVarTarget* m_target;
    int           m_index;
    int           m_pad08;
    bool          m_value;
    bool          m_dirty;
    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    GuiVarBoolSet(this->m_target, this->m_index, this->m_value);
    this->m_dirty = false;
}

int main()
{
    GuiVarTarget tgt; tgt.tag = 7;
    CGuiVarTransferBool o;
    o.m_target = &tgt;
    o.m_index  = 42;
    o.m_pad08  = 0;
    o.m_value  = true;
    o.m_dirty  = true;

    o.TransferToGui();

    bool ok = (g_target == &tgt) && (g_index == 42) && (g_value == 1) && (o.m_dirty == false);

    // second case: value false
    o.m_value = false;
    o.m_dirty = true;
    o.m_index = 5;
    o.TransferToGui();
    ok = ok && (g_index == 5) && (g_value == 0) && (o.m_dirty == false);

    if (ok) printf("TRANSFERTOGUI_OK\n");
    else    printf("FAIL\n");
    return ok ? 0 : 1;
}