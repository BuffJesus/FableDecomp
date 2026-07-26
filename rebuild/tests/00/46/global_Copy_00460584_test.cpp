#include "rebuild_abi.h"
#include <cstdio>

struct CSpecialAbilitiesDrunkennessDef { int x; };

// Provide the extern callee so the test links.
static int g_called = 0;
void __fastcall CSpecialAbilitiesDrunkennessDef_CopyImpl(
    CSpecialAbilitiesDrunkennessDef* thisptr, void* other)
{
    (void)other;
    if (thisptr) thisptr->x = 0x460584;
    g_called = 1;
}

extern void __fastcall CSpecialAbilitiesDrunkennessDef_Copy(
    CSpecialAbilitiesDrunkennessDef* thisptr, void* other);

int main(void)
{
    CSpecialAbilitiesDrunkennessDef a;
    a.x = 0;
    CSpecialAbilitiesDrunkennessDef b;
    b.x = 7;
    CSpecialAbilitiesDrunkennessDef_Copy(&a, &b);
    if (g_called == 1 && a.x == 0x460584) {
        printf("OK_460584\n");
    } else {
        printf("FAIL_460584\n");
    }
    return 0;
}