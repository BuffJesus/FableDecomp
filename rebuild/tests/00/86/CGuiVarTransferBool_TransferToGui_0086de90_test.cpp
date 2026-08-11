#include <cstdio>

// Local stand-in for the engine helper the leaf forwards to.
static int   g_target;
static int   g_value;
static int   g_flag;
static int   g_calls;

void __fastcall CGuiVarTransferBool_setter(int target, int value, unsigned char flag)
{
    g_target = target;
    g_value  = value;
    g_flag   = flag;
    ++g_calls;
}

struct CGuiVarTransferBool
{
    int           m_target;
    int           m_value;
    int           m_pad08;
    unsigned char m_flag;
    unsigned char m_dirty;
    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    CGuiVarTransferBool_setter(this->m_target, this->m_value, this->m_flag);
    this->m_dirty = 0;
}

int main()
{
    CGuiVarTransferBool v;
    v.m_target = 0x11223344;
    v.m_value  = 0x55667788;
    v.m_pad08  = 0;
    v.m_flag   = 1;
    v.m_dirty  = 0xAB;

    v.TransferToGui();

    bool ok = (g_calls == 1)
           && (g_target == 0x11223344)
           && (g_value  == 0x55667788)
           && (g_flag   == 1)
           && (v.m_dirty == 0);

    // second run with flag=0
    v.m_flag  = 0;
    v.m_dirty = 0x7F;
    v.TransferToGui();
    ok = ok && (g_calls == 2) && (g_flag == 0) && (v.m_dirty == 0);

    printf(ok ? "TRANSFERBOOL_OK\n" : "TRANSFERBOOL_FAIL\n");
    return ok ? 0 : 1;
}