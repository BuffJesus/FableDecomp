#include <cstdio>

static void*         g_target;
static int           g_edx;
static unsigned char g_value;
static int           g_called;

void __fastcall GuiVarTransferBoolCommit(void* target, int edx, unsigned char value)
{
    g_target = target;
    g_edx    = edx;
    g_value  = value;
    g_called = 1;
}

struct CGuiVarTransferBool
{
    void*         m_target;
    int           m_edxArg;
    int           m_pad8;
    unsigned char m_value;
    unsigned char m_dirty;
    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    GuiVarTransferBoolCommit(this->m_target, this->m_edxArg, this->m_value);
    this->m_dirty = 0;
}

int main()
{
    int dummy = 0;
    CGuiVarTransferBool o;
    o.m_target = &dummy;
    o.m_edxArg = 0x1234;
    o.m_pad8   = 0;
    o.m_value  = 1;
    o.m_dirty  = 0xAB;

    o.TransferToGui();

    if (g_called == 1 && g_target == &dummy && g_edx == 0x1234 &&
        g_value == 1 && o.m_dirty == 0)
    {
        printf("TRANSFERTOGUI_OK\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}