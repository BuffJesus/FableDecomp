#include <cstdio>

struct CGuiVarTransferBool {
    void*         f0;
    int           f4;
    int           f8;
    bool          fC;
    bool          fD;
};

static void*         g_seen_self;
static int           g_seen_edx;
static int           g_seen_val;

void __fastcall CGuiVarTransferBool_Helper(void* self, int edx, bool value)
{
    g_seen_self = self;
    g_seen_edx  = edx;
    g_seen_val  = value;
}

void __fastcall CGuiVarTransferBool_TransferToGui(CGuiVarTransferBool* self);

int main()
{
    int sentinel = 0;
    CGuiVarTransferBool o;
    o.f0 = &sentinel;
    o.f4 = 0x1234;
    o.f8 = 0;
    o.fC = true;
    o.fD = true;

    CGuiVarTransferBool_TransferToGui(&o);

    bool ok = (g_seen_self == &sentinel)
           && (g_seen_edx == 0x1234)
           && (g_seen_val == 1)
           && (o.fD == 0);

    if (ok) std::printf("004cd650_TEST PASS\n");
    else    std::printf("FAIL self=%p edx=%x val=%x fD=%d\n",
                        g_seen_self, g_seen_edx, g_seen_val, (int)o.fD);
    return 0;
}