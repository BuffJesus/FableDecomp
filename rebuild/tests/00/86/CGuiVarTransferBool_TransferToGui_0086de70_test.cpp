#include <cstdio>

static void *g_a;
static void *g_b;
static unsigned char g_v;
static int g_calls;

extern void __fastcall GuiTransferBoolHelper(void *a, void *b, unsigned char v);
void __fastcall GuiTransferBoolHelper(void *a, void *b, unsigned char v)
{
    g_a = a; g_b = b; g_v = v; ++g_calls;
}

struct CGuiVarTransferBool
{
    void         *m_pTarget;
    void         *m_pSource;
    void         *m_pad08;
    unsigned char m_value;
    unsigned char m_dirty;
    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    GuiTransferBoolHelper(m_pTarget, m_pSource, m_value);
    m_dirty = 0;
}

int main()
{
    CGuiVarTransferBool o;
    int tgt = 0x1111;
    int src = 0x2222;
    o.m_pTarget = &tgt;
    o.m_pSource = &src;
    o.m_pad08   = 0;
    o.m_value   = 0x5A;
    o.m_dirty   = 1;

    o.TransferToGui();

    bool ok = (g_calls == 1) &&
              (g_a == &tgt) &&
              (g_b == &src) &&
              (g_v == 0x5A) &&
              (o.m_dirty == 0);

    if (ok) { printf("TRANSFERBOOL_OK\n"); return 0; }
    printf("FAIL\n");
    return 1;
}