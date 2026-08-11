#include <cstdio>

struct GuiTarget;

void __fastcall GuiTargetSet(GuiTarget* self, int idx, unsigned char val);

struct CGuiVarTransferBool
{
    GuiTarget* target;   // +0
    int        idx;      // +4
    int        pad8;     // +8
    unsigned char value; // +0xc
    unsigned char flag;  // +0xd

    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    GuiTargetSet(this->target, this->idx, this->value);
    this->flag = 0;
}

// capture
static void*        g_self;
static int          g_idx;
static unsigned int g_val;

void __fastcall GuiTargetSet(GuiTarget* self, int idx, unsigned char val)
{
    g_self = self;
    g_idx  = idx;
    g_val  = val;
}

int main()
{
    CGuiVarTransferBool o;
    o.target = (GuiTarget*)0x12345678;
    o.idx    = 77;
    o.value  = 0xAB;
    o.flag   = 1;

    o.TransferToGui();

    bool ok = (g_self == (void*)0x12345678)
           && (g_idx == 77)
           && (g_val == 0xABu)
           && (o.flag == 0);

    printf(ok ? "TRANSFERBOOL_OK\n" : "TRANSFERBOOL_FAIL\n");
    return ok ? 0 : 1;
}