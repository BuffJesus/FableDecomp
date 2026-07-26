#include <cstdio>

struct CSpecialAbilitiesHealLifeDef;

// Provide the extern the source tail-calls, so the test links standalone.
static int g_called = 0;
extern "C" void impl_stub() { }

// Re-declare with a definition for the local link so main() can exercise
// the forwarding shape deterministically.
void __fastcall CSpecialAbilitiesHealLifeDef_Copy_impl(
    CSpecialAbilitiesHealLifeDef* /*thisptr*/, void* /*src*/)
{
    g_called = 1;
}

void __fastcall CSpecialAbilitiesHealLifeDef_Copy(
    CSpecialAbilitiesHealLifeDef* thisptr, void* src);

int main()
{
    CSpecialAbilitiesHealLifeDef* self = (CSpecialAbilitiesHealLifeDef*)0x1000;
    void* src = (void*)0x2000;
    CSpecialAbilitiesHealLifeDef_Copy(self, src);
    if (g_called == 1)
        printf("OK_THUNK_0046012c\n");
    else
        printf("FAIL_0046012c\n");
    return 0;
}