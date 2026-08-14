#include <cstdio>

struct CWideString;
static int g_calls = 0;
static void* g_self = 0;
static long g_a=0,g_b=0,g_c=0,g_zero=99;

extern "C" void __fastcall ConstructString_helper(
    CWideString* self, char* pByte, long a, long b, long c, long zero)
{
    (void)pByte;
    g_calls++;
    g_self = (void*)self;
    g_a=a; g_b=b; g_c=c; g_zero=zero;
}

CWideString* __fastcall ConstructString(
    CWideString* self, void* edxDummy, long a, long b, long c)
{
    (void)edxDummy;
    char local;
    ConstructString_helper(self, &local, a, b, c, 0);
    return self;
}

int main()
{
    CWideString* fakeThis = (CWideString*)0xDEAD1000;
    CWideString* r = ConstructString(fakeThis, 0, 11, 22, 33);
    if (r == fakeThis && g_calls == 1 && g_self == fakeThis &&
        g_a == 11 && g_b == 22 && g_c == 33 && g_zero == 0) {
        printf("BEHAVIOR_OK\n");
    } else {
        printf("BEHAVIOR_FAIL\n");
    }
    return 0;
}