#include <cstdio>

struct CGuiVarTransferBool
{
    void* m_target;
    int   m_edxval;
    int   m_pad08;
    bool  m_value;
    bool  m_written;
    void TransferToGui();
};

// Capture what the callee received.
static void*         g_self  = 0;
static int           g_edx   = 0;
static int           g_val   = -1;

extern "C" void __fastcall Transfer_a73b20(void* self, int edxarg, bool val)
{
    g_self = self;
    g_edx  = edxarg;
    g_val  = val ? 1 : 0;
}

void CGuiVarTransferBool::TransferToGui()
{
    Transfer_a73b20(m_target, m_edxval, m_value);
    m_written = 0;
}

int main()
{
    int dummyTarget = 0;
    CGuiVarTransferBool o;
    o.m_target  = &dummyTarget;
    o.m_edxval  = 0x1234;
    o.m_pad08   = 0;
    o.m_value   = true;
    o.m_written = true;

    o.TransferToGui();

    bool ok = (g_self == &dummyTarget)
           && (g_edx == 0x1234)
           && (g_val == 1)
           && (o.m_written == 0);

    if (ok)
        printf("TRANSFERTOGUI_OK\n");
    else
        printf("TRANSFERTOGUI_FAIL self=%p edx=%x val=%x written=%d\n",
               g_self, g_edx, g_val, (int)o.m_written);
    return ok ? 0 : 1;
}