#include <cstdio>

struct CGuiVarTransferBool
{
    void *m_pTarget;
    int   m_data;
    int   m_pad08;
    bool m_value;
    unsigned char m_dirty;
    void TransferToGui();
};

static void *g_target;
static int   g_edx;
static bool  g_val;
static int   g_called;

void __fastcall CGuiVarTransferBool_Apply(void *pTarget, int edxData, bool value)
{
    g_target = pTarget;
    g_edx = edxData;
    g_val = value;
    g_called = 1;
}

int main()
{
    CGuiVarTransferBool o;
    o.m_pTarget = (void *)0x1234;
    o.m_data    = 0x5678;
    o.m_pad08   = 0;
    o.m_value   = 1;
    o.m_dirty   = 1;

    o.TransferToGui();

    int ok = g_called == 1
          && g_target == (void *)0x1234
          && g_edx == 0x5678
          && g_val == true
          && o.m_dirty == 0;

    if (ok) { printf("TRANSFERBOOL_OK\n"); return 0; }
    printf("FAIL\n");
    return 1;
}