#include <cstdio>

struct GuiTarget {
    void __fastcall Set(int a, unsigned char b);
};

struct CGuiVarTransferBool {
    GuiTarget* target;   // +0x00
    int        param;    // +0x04
    char       pad8[4];  // +0x08
    unsigned char value; // +0x0c
    char       flag;     // +0x0d

    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    this->target->Set(this->param, this->value);
    this->flag = 0;
}

static int   g_seenA = -1;
static int   g_seenB = -1;
static void* g_seenThis = 0;

void __fastcall GuiTarget::Set(int a, unsigned char b)
{
    g_seenThis = this;
    g_seenA = a;
    g_seenB = (int)b;
}

int main()
{
    GuiTarget tgt;
    CGuiVarTransferBool v;
    v.target = &tgt;
    v.param  = 0x1234;
    v.value  = 0xAB;
    v.flag   = 7;

    v.TransferToGui();

    bool ok = (g_seenThis == &tgt)
           && (g_seenA == 0x1234)
           && (g_seenB == 0xAB)
           && (v.flag == 0);

    printf(ok ? "TRANSFERBOOL_OK\n" : "TRANSFERBOOL_FAIL a=%d b=%d flag=%d\n",
           g_seenA, g_seenB, (int)v.flag);
    return ok ? 0 : 1;
}