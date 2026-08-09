#include <cstdio>
struct CHoverable;
static CHoverable* g_hover;

static void model(CHoverable* self)
{
    g_hover = self;
    if (self == 0)
        g_hover = (CHoverable*)0x143bb34;
}

int main()
{
    CHoverable* p = (CHoverable*)0x1000;
    model(p);
    if (g_hover != p) { printf("FAIL nonzero\n"); return 1; }
    model(0);
    if (g_hover != (CHoverable*)0x143bb34) { printf("FAIL zero\n"); return 1; }
    printf("OK_00c166d0\n");
    return 0;
}