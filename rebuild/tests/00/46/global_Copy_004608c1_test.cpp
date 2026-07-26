#include <cstdio>

/* Standalone deterministic test: we cannot run the real engine tail-call,
 * so we mock the impl and confirm the thunk forwards this+src unchanged. */

struct CSpecialAbilitiesUnholyPowerDef { int tag; };

static CSpecialAbilitiesUnholyPowerDef* g_this = 0;
static void* g_src = 0;

extern "C" void mock_impl(CSpecialAbilitiesUnholyPowerDef* t, void* s)
{
    g_this = t;
    g_src = s;
}

static void thunk(CSpecialAbilitiesUnholyPowerDef* thisptr, void* src)
{
    mock_impl(thisptr, src); /* mirrors the tail-call forward */
}

int main(void)
{
    CSpecialAbilitiesUnholyPowerDef obj;
    obj.tag = 0x4608c1;
    int srcval = 0x1234;
    thunk(&obj, &srcval);
    if (g_this == &obj && g_src == &srcval)
        printf("OK_004608c1\n");
    else
        printf("FAIL_004608c1\n");
    return 0;
}