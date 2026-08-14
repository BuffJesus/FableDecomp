#include <stdio.h>

struct CInner { int dummy; };

struct CTCVillage
{
    CInner* m0;
    void*   m4;
    unsigned char m8, m9, ma, mb, mc, md;
};

static CInner* g_ecx;
static void*   g_edx;
static unsigned int g_flag;
static int g_calls;

void __fastcall InnerActivate(CInner* self, void* p, unsigned char flag)
{
    g_ecx = self; g_edx = p; g_flag = flag; g_calls++;
}

void __fastcall OnInitialActivate(CTCVillage* self)
{
    if (self->md)
    {
        InnerActivate(self->m0, self->m4, self->mc);
        self->md = 0;
    }
}

int main()
{
    CInner inner;
    CTCVillage v;
    v.m0 = &inner; v.m4 = (void*)0x1234; v.mc = 0xAB; v.md = 1;

    // guard set: call happens, args forwarded, flag cleared
    OnInitialActivate(&v);
    if (g_calls != 1) { printf("FAIL calls\n"); return 1; }
    if (g_ecx != &inner) { printf("FAIL ecx\n"); return 1; }
    if (g_edx != (void*)0x1234) { printf("FAIL edx\n"); return 1; }
    if (g_flag != 0xAB) { printf("FAIL flag zero-extend\n"); return 1; }
    if (v.md != 0) { printf("FAIL not cleared\n"); return 1; }

    // guard clear: no call
    v.md = 0; g_calls = 0;
    OnInitialActivate(&v);
    if (g_calls != 0) { printf("FAIL guard\n"); return 1; }

    printf("BEHAVE_OK_004bbbe0\n");
    return 0;
}