#include "rebuild_abi.h"
#include <cstdio>

struct CAppearanceDef { int dummy; };

/* Stub the forwarded-to implementation so the test links & runs. */
extern "C" void mark_called(void);
static int g_called = 0;

void __fastcall CAppearanceDef_Copy_impl(CAppearanceDef* thisptr, CAppearanceDef* src)
{
    (void)thisptr; (void)src;
    g_called = 1;
}

extern void __fastcall CAppearanceDef_Copy(CAppearanceDef* thisptr, CAppearanceDef* src);

int main(void)
{
    CAppearanceDef dst; dst.dummy = 0;
    CAppearanceDef src; src.dummy = 7;
    CAppearanceDef_Copy(&dst, &src);
    if (g_called == 1)
        printf("OK_004717c2\n");
    else
        printf("FAIL_004717c2\n");
    return 0;
}