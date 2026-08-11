#include <cstdio>

struct CGuiVarTransferBool
{
    void*          m_pTarget;
    void*          m_pVar;
    unsigned char  m_pad08[4];
    bool           m_value;
    bool           m_dirty;
    void TransferToGui();
};

static void* g_p1;
static void* g_p2;
static bool g_val;
static int g_called;

void __fastcall TransferBoolHelper(void* p1, void* p2, bool val)
{
    g_p1 = p1; g_p2 = p2; g_val = val; ++g_called;
}

void CGuiVarTransferBool::TransferToGui()
{
    TransferBoolHelper(m_pTarget, m_pVar, m_value);
    m_dirty = 0;
}

int main()
{
    int tgt = 0, var = 0;
    CGuiVarTransferBool o;
    o.m_pTarget = &tgt;
    o.m_pVar    = &var;
    o.m_value   = true;
    o.m_dirty   = true;

    o.TransferToGui();

    bool ok = g_called == 1 && g_p1 == &tgt && g_p2 == &var
              && g_val == true && o.m_dirty == false;

    // second run with value false
    o.m_value = false; o.m_dirty = true;
    o.TransferToGui();
    ok = ok && g_val == false && o.m_dirty == false && g_called == 2;

    printf(ok ? "TRANSFERTOGUI_OK\n" : "TRANSFERTOGUI_FAIL\n");
    return ok ? 0 : 1;
}