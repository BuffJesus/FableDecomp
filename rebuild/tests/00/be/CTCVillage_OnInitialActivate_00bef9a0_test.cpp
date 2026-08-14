#include <cstdio>

struct CObj { int tag; };

static CObj* g_obj;
static void* g_edx;
static unsigned char g_v;
static int g_calls;

void __fastcall Callee(CObj* obj, void* edxarg, unsigned char v)
{
    g_obj = obj; g_edx = edxarg; g_v = v; g_calls++;
}

struct CTCVillage {
    CObj*  f0;
    void*  f4;
    int    pad8;
    unsigned char fC;
    unsigned char fD;
};

void __fastcall OnInitialActivate(CTCVillage* self)
{
    if (self->fD) {
        Callee(self->f0, self->f4, self->fC);
        self->fD = 0;
    }
}

int main()
{
    CObj o; o.tag = 7;
    int edxmark = 0;

    // case: fD set -> calls callee, clears fD
    CTCVillage v;
    v.f0 = &o; v.f4 = &edxmark; v.pad8 = 0; v.fC = 0x2a; v.fD = 1;
    OnInitialActivate(&v);
    bool ok1 = (g_calls == 1) && (g_obj == &o) && (g_edx == &edxmark)
               && (g_v == 0x2a) && (v.fD == 0);

    // case: fD clear -> no call, fD stays 0
    g_calls = 0;
    v.fD = 0;
    OnInitialActivate(&v);
    bool ok2 = (g_calls == 0) && (v.fD == 0);

    if (ok1 && ok2) printf("PARITY_OK\n");
    else printf("FAIL c=%d v=%d fd=%d\n", g_calls, (int)g_v, (int)v.fD);
    return 0;
}