#include <cstdio>

struct CGuiVarBoolTarget;

struct CGuiVarTransferBool
{
    CGuiVarBoolTarget* m_pTarget;  // +0x00
    int                m_arg;      // +0x04
    int                m_pad08;    // +0x08
    unsigned char      m_value;    // +0x0c
    unsigned char      m_dirty;    // +0x0d

    void TransferToGui();
};

// captured call effects
static void* g_self;
static int   g_arg;
static int   g_val;
static int   g_calls;

void __fastcall CGuiVarBool_Apply(CGuiVarBoolTarget* self, int arg2, unsigned char val)
{
    g_self = (void*)self;
    g_arg  = arg2;
    g_val  = val;
    g_calls++;
}

void CGuiVarTransferBool::TransferToGui()
{
    CGuiVarBool_Apply(this->m_pTarget, this->m_arg, this->m_value);
    this->m_dirty = 0;
}

int main()
{
    CGuiVarTransferBool o;
    int dummyTarget = 0;
    o.m_pTarget = (CGuiVarBoolTarget*)&dummyTarget;
    o.m_arg   = 0x1234;
    o.m_pad08 = 0;
    o.m_value = 1;
    o.m_dirty = 0xAB;

    o.TransferToGui();

    int ok = (g_calls == 1)
          && (g_self == (void*)&dummyTarget)
          && (g_arg  == 0x1234)
          && (g_val  == 1)
          && (o.m_dirty == 0);

    // second case: value 0
    o.m_value = 0;
    o.m_dirty = 0x77;
    o.TransferToGui();
    ok = ok && (g_calls == 2) && (g_val == 0) && (o.m_dirty == 0);

    if (ok) { printf("TRANSFERBOOL_OK\n"); return 0; }
    printf("FAIL calls=%d arg=%d val=%d dirty=%d\n", g_calls, g_arg, g_val, o.m_dirty);
    return 1;
}