#include <cstdio>

struct Sub {};
struct CWideString { char pad[8]; Sub sub; };

static Sub*  g_self;
static long  g_edx;
static long  g_astk;
static int   g_zero;
static long  g_b;

extern "C" bool __fastcall Sub_Construct(Sub* self, long edx_a, long a_stack, int zero, long b)
{
    g_self = self; g_edx = edx_a; g_astk = a_stack; g_zero = zero; g_b = b;
    return true;
}

extern "C" bool __fastcall ConstructString(CWideString* self, int edx, long a, long b);

int main()
{
    CWideString w;
    bool r = ConstructString(&w, 0, 111, 222);

    bool ok = r
        && g_self == &w.sub
        && g_edx  == 111
        && g_astk == 111
        && g_zero == 0
        && g_b    == 222;

    if (ok) printf("00c25ff0_TEST PASS\n");
    else    printf("FAIL self=%p sub=%p edx=%ld astk=%ld zero=%d b=%ld r=%d\n",
                   (void*)g_self,(void*)&w.sub,g_edx,g_astk,g_zero,g_b,(int)r);
    return 0;
}