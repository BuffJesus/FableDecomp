#include <cstdio>

struct CGuiVarTransferBool
{
    void *f0;
    int   f4;
    char  pad8[4];
    bool  fc;
    bool  fd;

    void TransferToGui();
};

static void *g_self;
static int   g_val;
static int   g_flag;

void __fastcall CGuiVarTransferBool_sink(void *self, int val, bool flag)
{
    g_self = self;
    g_val  = val;
    g_flag = flag ? 1 : 0;
}

int main()
{
    int target = 0x1234;
    void *ptarget = &target ;
    CGuiVarTransferBool o;
    o.f0 = ptarget;
    o.f4 = 42;
    o.fc = true;
    o.fd = true;

    o.TransferToGui();

    bool ok = (g_self == ptarget) && (g_val == 42) && (g_flag == 1) && (o.fd == false);

    o.fc = false;
    o.fd = true;
    o.f4 = -5;
    o.TransferToGui();
    ok = ok && (g_val == -5) && (g_flag == 0) && (o.fd == false);

    printf(ok ? "TRANSFERBOOL_OK\n" : "FAIL\n");
    return ok ? 0 : 1;
}