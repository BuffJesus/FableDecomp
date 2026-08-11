#include <cstdio>

struct CGuiVarTransferBool
{
    void* m_pTarget;
    int   m_edxArg;
    int   m_pad08;
    bool  m_value;
    unsigned char m_dirty;
    void TransferToGui();
};

static void*        g_seenTarget;
static int          g_seenEdx;
static int          g_seenValue;
static int          g_callCount;

void __fastcall CGuiVarTransferBool_Apply(void* pTarget, int edxArg, bool value)
{
    g_seenTarget = pTarget;
    g_seenEdx    = edxArg;
    g_seenValue  = value;
    ++g_callCount;
}

void CGuiVarTransferBool::TransferToGui()
{
    CGuiVarTransferBool_Apply(this->m_pTarget, this->m_edxArg, this->m_value);
    this->m_dirty = 0;
}

int main()
{
    int sentinel = 0;
    CGuiVarTransferBool o;
    o.m_pTarget = &sentinel;
    o.m_edxArg  = 0x1234;
    o.m_pad08   = 0;
    o.m_value   = 1;
    o.m_dirty   = 0xAB;

    o.TransferToGui();

    bool ok = (g_callCount == 1)
           && (g_seenTarget == &sentinel)
           && (g_seenEdx == 0x1234)
           && (g_seenValue == 1)
           && (o.m_dirty == 0);

    // second: value 0
    o.m_value = 0;
    o.m_dirty = 0x7F;
    o.TransferToGui();
    ok = ok && (g_seenValue == 0) && (o.m_dirty == 0) && (g_callCount == 2);

    if (ok) printf("TRANSFERTOGUI_OK\n");
    else    printf("FAIL c=%d v=%d d=%d\n", g_callCount, g_seenValue, (int)o.m_dirty);
    return ok ? 0 : 1;
}