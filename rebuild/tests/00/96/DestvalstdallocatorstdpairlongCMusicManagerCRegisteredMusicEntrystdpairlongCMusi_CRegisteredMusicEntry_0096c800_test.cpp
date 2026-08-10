#include <stdio.h>

static int g_sub_called = 0;
static int g_tail_called = 0;
static void* g_sub_arg = 0;
static void* g_tail_arg = 0;

struct Sub { char pad[4]; };
struct Obj {
    char pad[0xa8];
    Sub sub;
};

void helper_a01c10(void* p) { g_sub_called++; g_sub_arg = p; }
void helper_693ef0(void* p) { g_tail_called++; g_tail_arg = p; }

void model(void* self)
{
    Obj* o = (Obj*)self;
    helper_a01c10(&o->sub);
    helper_693ef0(o);
}

int main()
{
    Obj obj;
    model(&obj);
    if (g_sub_called != 1) { printf("FAIL sub_called=%d\n", g_sub_called); return 1; }
    if (g_tail_called != 1) { printf("FAIL tail_called=%d\n", g_tail_called); return 1; }
    if (g_sub_arg != (char*)&obj + 0xa8) { printf("FAIL sub_arg offset\n"); return 1; }
    if (g_tail_arg != (void*)&obj) { printf("FAIL tail_arg\n"); return 1; }
    printf("OK_0096c800\n");
    return 0;
}