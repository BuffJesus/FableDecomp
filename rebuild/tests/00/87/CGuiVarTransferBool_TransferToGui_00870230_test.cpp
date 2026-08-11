#include <cstdio>

struct CGuiTarget;

void __fastcall GuiSetBool(CGuiTarget* self, void* edx, unsigned char v);

struct CGuiVarTransferBool
{
    CGuiTarget* target;
    void*       cookie;
    int         pad08;
    unsigned char value;
    unsigned char dirty;
    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    GuiSetBool(this->target, this->cookie, this->value);
    this->dirty = 0;
}

// test harness capture
static CGuiTarget* g_self;
static void*       g_edx;
static unsigned char g_v = 0xEE;

void __fastcall GuiSetBool(CGuiTarget* self, void* edx, unsigned char v)
{
    g_self = self;
    g_edx  = edx;
    g_v    = v;
}

int main()
{
    CGuiVarTransferBool o;
    o.target = (CGuiTarget*)0x1234;
    o.cookie = (void*)0x5678;
    o.value  = 1;
    o.dirty  = 0xAB;
    o.TransferToGui();

    bool ok = (g_self == (CGuiTarget*)0x1234)
           && (g_edx  == (void*)0x5678)
           && (g_v    == 1)
           && (o.dirty == 0);
    if (ok) printf("TRANSFERBOOL_OK\n");
    else    printf("FAIL self=%p edx=%p v=%u dirty=%u\n", g_self, g_edx, g_v, o.dirty);
    return ok ? 0 : 1;
}