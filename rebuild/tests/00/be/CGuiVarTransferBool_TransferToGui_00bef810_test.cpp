#include <cstdio>

struct CGuiTarget;

struct CGuiVarTransferBool {
    CGuiTarget* target;
    int         param;
    char        pad8[4];
    bool        curValue;
    bool        dirty;
};

static void* g_self;
static int   g_edx;
static int   g_val;
static int   g_calls;

void __fastcall CGuiTarget_Set(CGuiTarget* self, int edx, bool value)
{
    g_self = self;
    g_edx = edx;
    g_val = value ? 1 : 0;
    g_calls++;
}

void __fastcall CGuiVarTransferBool_TransferToGui(CGuiVarTransferBool* self);

int main()
{
    CGuiVarTransferBool o;
    o.target = (CGuiTarget*)0x12345678;
    o.param  = 99;
    o.curValue = true;
    o.dirty  = true;

    CGuiVarTransferBool_TransferToGui(&o);

    bool ok = g_calls == 1
        && g_self == (void*)0x12345678
        && g_edx == 99
        && g_val == 1
        && o.dirty == false;

    if (ok) printf("TRANSFERTOGUI_OK\n");
    else    printf("FAIL calls=%d self=%p edx=%d val=%d dirty=%d\n",
                   g_calls, g_self, g_edx, g_val, (int)o.dirty);
    return ok ? 0 : 1;
}