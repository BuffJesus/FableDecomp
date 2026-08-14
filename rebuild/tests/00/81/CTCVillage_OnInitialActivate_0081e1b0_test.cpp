#include <cstdio>

struct Target { int calls; int lastArg; };

static Target* g_ecx;
static void*   g_edx;
static int     g_arg;
static int     g_callCount;

struct CTCVillage
{
    Target* field0;
    void*   field4;
    char    pad8;
    char    pad9;
    char    padA;
    char    padB;
    unsigned char fieldC;
    unsigned char fieldD;
};

void __fastcall Callee(Target* ecxObj, void* edxArg, int arg)
{
    g_ecx = ecxObj; g_edx = edxArg; g_arg = arg; g_callCount++;
}

void __fastcall OnInitialActivate(CTCVillage* self)
{
    if (self->fieldD)
    {
        Callee(self->field0, self->field4, self->fieldC);
        self->fieldD = 0;
    }
}

int main()
{
    Target t = {0,0};
    CTCVillage v;
    v.field0 = &t;
    int edxMarker = 0;
    v.field4 = &edxMarker;
    v.fieldC = 0x7a;
    v.fieldD = 1;

    OnInitialActivate(&v);
    // active path: called once, arg zero-extended, flag cleared
    if (g_callCount != 1) { printf("FAIL count\n"); return 1; }
    if (g_ecx != &t) { printf("FAIL ecx\n"); return 1; }
    if (g_edx != &edxMarker) { printf("FAIL edx\n"); return 1; }
    if (g_arg != 0x7a) { printf("FAIL arg\n"); return 1; }
    if (v.fieldD != 0) { printf("FAIL flag\n"); return 1; }

    // inactive path: flag already 0 -> no call
    OnInitialActivate(&v);
    if (g_callCount != 1) { printf("FAIL noskip\n"); return 1; }

    // high-bit byte zero-extends (not sign-extends)
    g_callCount = 0;
    v.fieldC = 0xff;
    v.fieldD = 1;
    OnInitialActivate(&v);
    if (g_arg != 0xff) { printf("FAIL zext\n"); return 1; }

    printf("PARITY_OK_0081e1b0\n");
    return 0;
}