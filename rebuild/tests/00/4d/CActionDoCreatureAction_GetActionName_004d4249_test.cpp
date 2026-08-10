#include <cstdio>

struct CCharString { const char* p; int n; };

static const char* g_lit = 0;
static int g_n = 0;

extern "C" CCharString* CtorFromLit(CCharString* self, int, const char* s, int n)
{
    self->p = s; self->n = n;
    g_lit = s; g_n = n;
    return self;
}

extern "C" CCharString* GetActionName_2_model(CCharString* self)
{
    CtorFromLit(self, 0, "the-literal", -1);
    return self;
}

int main()
{
    CCharString buf; buf.p = 0; buf.n = 0;
    CCharString* r = GetActionName_2_model(&buf);
    if (r != &buf) { printf("FAIL ret\n"); return 1; }
    if (g_n != -1) { printf("FAIL n\n"); return 1; }
    if (g_lit == 0) { printf("FAIL lit\n"); return 1; }
    printf("OK_004d4249\n");
    return 0;
}