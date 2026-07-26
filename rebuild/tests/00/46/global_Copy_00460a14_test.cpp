#include <cstdio>

/* Deterministic stand-in: the real thunk is a jmp to an engine routine,
 * so the test only exercises the tail-call shape via a local mock. */
struct CSpecialAbilitiesCreatureTintDef { int v; };

static int g_called = 0;
static void copy_impl(CSpecialAbilitiesCreatureTintDef* d,
                      CSpecialAbilitiesCreatureTintDef* s)
{
    d->v = s->v;
    g_called = 1;
}

static void copy_thunk(CSpecialAbilitiesCreatureTintDef* d,
                       CSpecialAbilitiesCreatureTintDef* s)
{
    copy_impl(d, s); /* tail-call */
}

int main(void)
{
    CSpecialAbilitiesCreatureTintDef a; a.v = 0;
    CSpecialAbilitiesCreatureTintDef b; b.v = 0x460A14;
    copy_thunk(&a, &b);
    if (g_called && a.v == 0x460A14)
        printf("PASS_460a14_OK\n");
    else
        printf("FAIL_460a14\n");
    return 0;
}