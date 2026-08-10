#include <cstdio>

typedef unsigned long ulong;

struct CTexture {
    void* field0;
};

static void* g_p;
static int   g_zero = -1;
static ulong g_b, g_a;

extern void __stdcall sub_bfefce(void* p, int zero, ulong b, ulong a);
void __stdcall sub_bfefce(void* p, int zero, ulong b, ulong a)
{
    g_p = p; g_zero = zero; g_b = b; g_a = a;
}

void __fastcall GenerateMipmaps(CTexture* self, int edx, ulong a, ulong b);

int main()
{
    CTexture t; t.field0 = (void*)0xDEAD;
    GenerateMipmaps(&t, 0, 11u, 22u);
    if (g_p == (void*)0xDEAD && g_zero == 0 && g_b == 22u && g_a == 11u)
        printf("009f9e60_TEST PASS\n");
    else
        printf("FAIL p=%p z=%d b=%lu a=%lu\n", g_p, g_zero, g_b, g_a);
    return 0;
}