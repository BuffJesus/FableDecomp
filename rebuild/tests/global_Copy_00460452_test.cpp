#include <cstdio>

struct CSpecialAbilitiesSummonSpellDef;

extern "C" void printf_hook();

// Provide a stub impl so the test links and runs deterministically.
static int g_called = 0;
void __fastcall CSpecialAbilitiesSummonSpellDef_Copy_impl(
    CSpecialAbilitiesSummonSpellDef* thisptr, void* src)
{
    (void)thisptr; (void)src;
    g_called = 1;
}

void __fastcall CSpecialAbilitiesSummonSpellDef_Copy(
    CSpecialAbilitiesSummonSpellDef* thisptr, void* src);

int main()
{
    CSpecialAbilitiesSummonSpellDef* self = (CSpecialAbilitiesSummonSpellDef*)0x1000;
    void* src = (void*)0x2000;
    CSpecialAbilitiesSummonSpellDef_Copy(self, src);
    if (g_called == 1)
        printf("PASS_460452_OK\n");
    else
        printf("FAIL_460452\n");
    return 0;
}