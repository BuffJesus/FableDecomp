#include <cstdio>

static int g_dst;
static int g_idx;
static int g_val;
static int g_calls;

void __fastcall SetGuiBoolValue(void* dst, int idx, bool value)
{
    g_dst = (int)(dst != 0);
    g_idx = idx;
    g_val = value ? 1 : 0;
    ++g_calls;
}

struct CGuiVarTransferBool
{
    void* m_a;
    int   m_b;
    int   m_pad;
    bool  m_value;
    bool  m_dirty;

    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    SetGuiBoolValue(m_a, m_b, m_value);
    m_dirty = false;
}

int main()
{
    CGuiVarTransferBool o;
    int slot = 0;
    void* pmark = &slot ;
    o.m_a = pmark;
    o.m_b = 77;
    o.m_pad = 0;
    o.m_value = true;
    o.m_dirty = true;

    o.TransferToGui();

    bool ok = (g_calls == 1) && (g_dst == 1) && (g_idx == 77) &&
              (g_val == 1) && (o.m_dirty == false);

    // second run with value false
    o.m_value = false;
    o.m_dirty = true;
    o.TransferToGui();
    ok = ok && (g_calls == 2) && (g_val == 0) && (o.m_dirty == false);

    if (ok) printf("TRANSFERBOOL_OK\n");
    else printf("TRANSFERBOOL_FAIL calls=%d dst=%d idx=%d val=%d dirty=%d\n",
                g_calls, g_dst, g_idx, g_val, (int)o.m_dirty);
    return ok ? 0 : 1;
}