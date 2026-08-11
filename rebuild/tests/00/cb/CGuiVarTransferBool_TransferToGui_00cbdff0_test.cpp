#include <cstdio>

struct GuiTargetObj;

static int g_lastValue = -999;
static void* g_lastObj = 0;
static void* g_lastEdx = 0;

void __fastcall SetGuiBoolValue(GuiTargetObj* obj, void* edxArg, unsigned char value)
{
    g_lastObj = (void*)obj;
    g_lastEdx = edxArg;
    g_lastValue = value;
}

struct CGuiVarTransferBool
{
    GuiTargetObj* pTarget;
    void*         edxArg;
    int           pad08;
    unsigned char curValue;
    unsigned char dirtyFlag;

    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    SetGuiBoolValue(this->pTarget, this->edxArg, this->curValue);
    this->dirtyFlag = 0;
}

int main()
{
    CGuiVarTransferBool o;
    o.pTarget = (GuiTargetObj*)0x11112222;
    o.edxArg  = (void*)0x33334444;
    o.pad08   = 0;
    o.curValue = 0xC7;
    o.dirtyFlag = 0xAA;

    o.TransferToGui();

    bool ok = (g_lastObj == (void*)0x11112222)
           && (g_lastEdx == (void*)0x33334444)
           && (g_lastValue == 0xC7)
           && (o.dirtyFlag == 0);

    if (ok) printf("TRANSFERBOOL_OK\n");
    else    printf("FAIL obj=%p edx=%p val=%d dirty=%d\n", g_lastObj, g_lastEdx, g_lastValue, (int)o.dirtyFlag);
    return ok ? 0 : 1;
}