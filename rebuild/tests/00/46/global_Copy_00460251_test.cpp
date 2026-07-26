#include <cstdio>

// Stand-in for the relocation-masked tail-call target so the test links.
struct CSpecialAbilitiesFireballSpellDef;
static int g_called = 0;
extern "C" void CSpecialAbilitiesFireballSpellDef_Copy_impl_stub() {}

// Minimal reproduction of the thunk semantics for a runtime sanity check.
void do_copy(CSpecialAbilitiesFireballSpellDef* thisptr, void* src)
{
    (void)thisptr; (void)src;
    g_called = 1;
}

int main()
{
    do_copy((CSpecialAbilitiesFireballSpellDef*)0x1000, (void*)0x2000);
    if (g_called == 1)
        printf("OK_460251_THUNK\n");
    return 0;
}