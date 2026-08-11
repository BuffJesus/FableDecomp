#include <cstdio>

struct CGuiVarBoolTarget;

struct CGuiVarTransferBool {
    CGuiVarBoolTarget* target;
    void*              arg4;
    int                pad8;
    unsigned char      value;
    unsigned char      dirty;
};

extern void __fastcall CGuiVarBool_Set(CGuiVarBoolTarget* self, void* edxArg, unsigned char val);
void __fastcall CGuiVarTransferBool_TransferToGui(CGuiVarTransferBool* self);

// Capture the arguments the setter receives.
static CGuiVarBoolTarget* g_self;
static void*              g_edx;
static int                g_val;
static int                g_calls;

void __fastcall CGuiVarBool_Set(CGuiVarBoolTarget* self, void* edxArg, unsigned char val)
{
    g_self  = self;
    g_edx   = edxArg;
    g_val   = val;
    g_calls++;
}

int main()
{
    CGuiVarTransferBool o;
    o.target = (CGuiVarBoolTarget*)0x11223344;
    o.arg4   = (void*)0x55667788;
    o.pad8   = 0x7fffffff;
    o.value  = 0xAB;   // 171
    o.dirty  = 0x99;

    CGuiVarTransferBool_TransferToGui(&o);

    bool ok = true;
    ok = ok && (g_calls == 1);
    ok = ok && (g_self  == (CGuiVarBoolTarget*)0x11223344);
    ok = ok && (g_edx   == (void*)0x55667788);
    ok = ok && (g_val   == 0xAB);          // zero-extended byte
    ok = ok && (o.dirty == 0);

    if (ok) {
        printf("TRANSFERBOOL_OK val=%d dirty=%d\n", g_val, (int)o.dirty);
        return 0;
    }
    printf("TRANSFERBOOL_FAIL calls=%d val=%d dirty=%d\n", g_calls, g_val, (int)o.dirty);
    return 1;
}