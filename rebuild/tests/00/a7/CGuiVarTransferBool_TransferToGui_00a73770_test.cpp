#include <cstdio>

struct GuiTarget;
struct GuiSource;

struct CGuiVarTransferBool
{
    GuiTarget *m_target;
    GuiSource *m_source;
    int        m_pad08;
    bool       m_value;
    bool       m_dirty;
    void TransferToGui();
};

static GuiTarget *g_lastEcx;
static GuiSource *g_lastEdx;
static bool       g_lastVal;
static int        g_calls;

void __fastcall SetGuiBoolValue(GuiTarget *ecxArg, GuiSource *edxArg, bool value)
{
    g_lastEcx = ecxArg;
    g_lastEdx = edxArg;
    g_lastVal = value;
    ++g_calls;
}

void CGuiVarTransferBool::TransferToGui()
{
    SetGuiBoolValue(m_target, m_source, m_value);
    m_dirty = false;
}

int main()
{
    CGuiVarTransferBool o;
    o.m_target = (GuiTarget*)0x11112222;
    o.m_source = (GuiSource*)0x33334444;
    o.m_pad08  = 0;
    o.m_value  = true;
    o.m_dirty  = true;

    g_calls = 0;
    o.TransferToGui();

    bool ok = (g_calls == 1)
           && (g_lastEcx == (GuiTarget*)0x11112222)
           && (g_lastEdx == (GuiSource*)0x33334444)
           && (g_lastVal == true)
           && (o.m_dirty == false);

    // second run with value=false
    o.m_value = false;
    o.m_dirty = true;
    o.TransferToGui();
    ok = ok && (g_calls == 2) && (g_lastVal == false) && (o.m_dirty == false);

    if (ok) printf("TRANSFERBOOL_TOGUI_OK\n");
    else    printf("FAIL calls=%d val=%d dirty=%d\n", g_calls, (int)g_lastVal, (int)o.m_dirty);
    return ok ? 0 : 1;
}