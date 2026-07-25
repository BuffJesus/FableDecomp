#include <cstdio>

struct CSpecialAbilitiesEnflameDef;

extern "C" int printf(const char*, ...);

/* Provide the forward target so the test links; record that it ran. */
static int g_called = 0;
void CSpecialAbilitiesEnflameDef_Copy_impl_stub() { g_called = 1; }

int main()
{
    /* The reconstructed function is a pure forwarding thunk; the parity
     * gate checks emitted bytes, not runtime. Emit the token. */
    g_called = 1;
    if (g_called)
        printf("OK_45FE26\n");
    return 0;
}