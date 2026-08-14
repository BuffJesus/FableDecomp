#include <cstdio>

struct CInner;

static int g_calledObj;
static int g_calledArg;
static int g_calledB;
static int g_callCount;

void __fastcall Inner_Do(CInner* obj, int arg1, unsigned char b)
{
    g_calledObj = (int)(long)obj;
    g_calledArg = arg1;
    g_calledB = (int)b;
    g_callCount++;
}

struct CTCVillage {
    CInner* obj;
    int     field4;
    char    pad8;
    char    pad9;
    char    padA;
    char    padB;
    unsigned char byteC;
    char    flagD;
};

void __fastcall OnInitialActivate(CTCVillage* self)
{
    if (self->flagD) {
        Inner_Do(self->obj, self->field4, self->byteC);
        self->flagD = 0;
    }
}

int main()
{
    CTCVillage v;
    v.obj = (CInner*)0x1234;
    v.field4 = 99;
    v.byteC = 0xAB;

    v.flagD = 0;
    g_callCount = 0;
    OnInitialActivate(&v);
    if (g_callCount != 0) { printf("FAIL: called when flag clear\n"); return 1; }

    v.flagD = 1;
    OnInitialActivate(&v);
    if (g_callCount != 1) { printf("FAIL: not called when flag set\n"); return 1; }
    if (g_calledObj != 0x1234) { printf("FAIL: obj\n"); return 1; }
    if (g_calledArg != 99) { printf("FAIL: arg\n"); return 1; }
    if (g_calledB != 0xAB) { printf("FAIL: byte\n"); return 1; }
    if (v.flagD != 0) { printf("FAIL: flag not cleared\n"); return 1; }

    OnInitialActivate(&v);
    if (g_callCount != 1) { printf("FAIL: called again\n"); return 1; }

    printf("PARITY_OK\n");
    return 0;
}