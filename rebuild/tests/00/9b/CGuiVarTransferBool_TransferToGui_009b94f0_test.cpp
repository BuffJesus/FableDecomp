#include <cstdio>

void __fastcall GuiVarSetBool(void* pFirst, int pSecond, unsigned char v);

struct CGuiVarTransferBool
{
    void*         m_pFirst;
    int           m_pSecond;
    int           m_pad08;
    unsigned char m_value;
    unsigned char m_flag;

    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    GuiVarSetBool(this->m_pFirst, this->m_pSecond, this->m_value);
    this->m_flag = 0;
}

static void*         g_seenFirst;
static int           g_seenSecond;
static unsigned char g_seenValue;
static int           g_calls;

void __fastcall GuiVarSetBool(void* pFirst, int pSecond, unsigned char v)
{
    g_seenFirst  = pFirst;
    g_seenSecond = pSecond;
    g_seenValue  = v;
    ++g_calls;
}

int main()
{
    int dummy = 0;
    CGuiVarTransferBool o;
    o.m_pFirst  = &dummy;
    o.m_pSecond = 0x12345678;
    o.m_pad08   = 0;
    o.m_value   = 0x5A;
    o.m_flag    = 0x77;

    o.TransferToGui();

    bool ok = (g_calls == 1)
           && (g_seenFirst  == &dummy)
           && (g_seenSecond == 0x12345678)
           && (g_seenValue  == 0x5A)
           && (o.m_flag     == 0);

    if (ok) { std::printf("TRANSFERBOOL_OK\n"); return 0; }
    std::printf("TRANSFERBOOL_FAIL calls=%d val=%d flag=%d\n", g_calls, (int)g_seenValue, (int)o.m_flag);
    return 1;
}