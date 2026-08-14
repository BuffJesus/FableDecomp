#include <cstdio>

struct CTCVillage {
    void*         f0;
    void*         f4;
    char          pad8[4];
    bool          byteC;
    unsigned char flagD;
};

static int   g_calls = 0;
static bool  g_lastArg = false;
static void* g_lastEcx = 0;
static void* g_lastEdx = 0;

void __fastcall Village_Sub(void* ecx, void* edx, bool arg)
{
    g_calls++;
    g_lastEcx = ecx;
    g_lastEdx = edx;
    g_lastArg = arg;
}

void __fastcall OnInitialActivate(CTCVillage* self)
{
    if (self->flagD) {
        Village_Sub(self->f0, self->f4, self->byteC);
        self->flagD = 0;
    }
}

int main()
{
    // Case 1: latch set -> calls callee, forwards f0/f4/byteC, clears latch
    CTCVillage a;
    a.f0 = (void*)0x1111;
    a.f4 = (void*)0x2222;
    a.byteC = true;
    a.flagD = 1;
    OnInitialActivate(&a);
    if (g_calls != 1)                { printf("FAIL calls\n"); return 1; }
    if (a.flagD != 0)                { printf("FAIL latch not cleared\n"); return 1; }
    if (g_lastEcx != (void*)0x1111)  { printf("FAIL ecx\n"); return 1; }
    if (g_lastEdx != (void*)0x2222)  { printf("FAIL edx\n"); return 1; }
    if (g_lastArg != true)           { printf("FAIL arg\n"); return 1; }

    // Case 2: byteC false still forwarded
    CTCVillage c;
    c.f0 = (void*)0x9; c.f4 = (void*)0xA; c.byteC = false; c.flagD = 1;
    g_calls = 0;
    OnInitialActivate(&c);
    if (g_calls != 1)      { printf("FAIL calls2\n"); return 1; }
    if (g_lastArg != false){ printf("FAIL arg2\n"); return 1; }

    // Case 3: latch clear -> no call, no state change
    CTCVillage b;
    b.f0 = 0; b.f4 = 0; b.byteC = true; b.flagD = 0;
    g_calls = 0;
    OnInitialActivate(&b);
    if (g_calls != 0) { printf("FAIL should not call\n"); return 1; }
    if (b.flagD != 0) { printf("FAIL flag\n"); return 1; }

    printf("PARITY_OK_a420c0\n");
    return 0;
}