#include <cstdio>

struct GuiTarget { int captured_idx; unsigned char captured_val; int touched; };

static GuiTarget *g_lastObj = 0;
static int g_lastIdx = -1;
static unsigned char g_lastVal = 0xEE;

void __fastcall SetGuiValue(GuiTarget *obj, int idx, unsigned char val)
{
    g_lastObj = obj;
    g_lastIdx = idx;
    g_lastVal = val;
    if (obj) { obj->captured_idx = idx; obj->captured_val = val; obj->touched = 1; }
}

struct CGuiVarTransferBool
{
    GuiTarget *m_target;
    int        m_index;
    int        m_pad8;
    unsigned char m_value;
    unsigned char m_done;
    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    SetGuiValue(m_target, m_index, m_value);
    m_done = 0;
}

int main()
{
    GuiTarget tgt = {0, 0, 0};
    CGuiVarTransferBool v;
    v.m_target = &tgt;
    v.m_index  = 42;
    v.m_pad8   = 0x11223344;
    v.m_value  = 1;
    v.m_done   = 0x7F;

    v.TransferToGui();

    bool ok = (g_lastObj == &tgt) && (g_lastIdx == 42) && (g_lastVal == 1)
              && (tgt.captured_idx == 42) && (tgt.captured_val == 1)
              && (tgt.touched == 1) && (v.m_done == 0);

    // second case: value 0, other target
    GuiTarget tgt2 = {0,0,0};
    CGuiVarTransferBool v2;
    v2.m_target = &tgt2;
    v2.m_index  = 7;
    v2.m_value  = 0;
    v2.m_done   = 0x55;
    v2.TransferToGui();
    ok = ok && (g_lastVal == 0) && (g_lastIdx == 7) && (v2.m_done == 0);

    if (ok) printf("TRANSFERBOOL_OK\n");
    else    printf("TRANSFERBOOL_FAIL idx=%d val=%d done=%d\n", g_lastIdx, g_lastVal, v.m_done);
    return ok ? 0 : 1;
}