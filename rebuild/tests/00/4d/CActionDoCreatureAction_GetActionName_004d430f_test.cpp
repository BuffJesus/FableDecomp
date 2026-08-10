#include <cstdio>

struct CCharString { void* p; };

static int g_calls = 0;
static const char* g_s = 0;
static int g_n = 0;

static void Helper_model(CCharString* self, const char* s, int n)
{
    g_calls++;
    g_s = s;
    g_n = n;
}

static CCharString* GetActionName_4(CCharString* self)
{
    Helper_model(self, (const char*)0x123baec, -1);
    return self;
}

int main()
{
    CCharString buf;
    buf.p = (void*)0xdeadbeef;
    CCharString* r = GetActionName_4(&buf);
    if (r != &buf) { printf("FAIL ret\n"); return 1; }
    if (g_calls != 1) { printf("FAIL calls\n"); return 1; }
    if (g_s != (const char*)0x123baec) { printf("FAIL s\n"); return 1; }
    if (g_n != -1) { printf("FAIL n\n"); return 1; }
    printf("OK_004d430f\n");
    return 0;
}