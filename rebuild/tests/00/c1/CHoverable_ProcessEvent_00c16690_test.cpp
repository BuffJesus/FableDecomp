#include <stdio.h>
struct CHoverable;
static CHoverable* g_hover = 0;
void model_ProcessEvent(CHoverable* self)
{
    g_hover = self;
    if (!self)
        g_hover = (CHoverable*)0x143bb34;
}
int main()
{
    CHoverable* a = (CHoverable*)0x1234;
    model_ProcessEvent(a);
    if (g_hover != a) { printf("FAIL_nonnull\n"); return 1; }
    model_ProcessEvent(0);
    if (g_hover != (CHoverable*)0x143bb34) { printf("FAIL_null\n"); return 1; }
    printf("OK_00c16690\n");
    return 0;
}