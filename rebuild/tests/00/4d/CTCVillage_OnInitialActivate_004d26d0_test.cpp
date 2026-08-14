#include <cstdio>

struct Callee;

static Callee* g_lastSelf;
static void*   g_lastArg;
static unsigned char g_lastFlag;
static int g_calls;

extern "C" void __fastcall Callee_Do(Callee* self, void* arg2, unsigned char flag)
{
    g_lastSelf = self; g_lastArg = arg2; g_lastFlag = flag; ++g_calls;
}

struct CTCVillage {
    Callee* m_pCallee;
    void*   m_arg;
    unsigned char pad08, pad09, pad0a, pad0b;
    unsigned char m_byteC;
    unsigned char m_flagD;
};

void __fastcall OnInitialActivate(CTCVillage* self)
{
    if (self->m_flagD != 0)
    {
        Callee_Do(self->m_pCallee, self->m_arg, self->m_byteC);
        self->m_flagD = 0;
    }
}

int main()
{
    CTCVillage v;
    // Case 1: flag set -> call happens, args forwarded, flag cleared
    v.m_pCallee = (Callee*)0x1111;
    v.m_arg     = (void*)0x2222;
    v.m_byteC   = 0x7F;
    v.m_flagD   = 1;
    g_calls = 0;
    OnInitialActivate(&v);
    if (g_calls != 1) { printf("BAD calls\n"); return 1; }
    if (g_lastSelf != (Callee*)0x1111) { printf("BAD self\n"); return 1; }
    if (g_lastArg != (void*)0x2222) { printf("BAD arg\n"); return 1; }
    if (g_lastFlag != 0x7F) { printf("BAD flag\n"); return 1; }
    if (v.m_flagD != 0) { printf("BAD notcleared\n"); return 1; }

    // Case 2: flag clear -> no call
    v.m_flagD = 0;
    g_calls = 0;
    OnInitialActivate(&v);
    if (g_calls != 0) { printf("BAD spurious\n"); return 1; }

    printf("PARITY_OK\n");
    return 0;
}