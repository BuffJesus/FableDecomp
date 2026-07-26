#include <cstdio>

struct CArmourDef;

/* provide the impl the thunk jumps to so the test links & runs */
extern "C" void mark_called();
int g_called = 0;

void __fastcall CArmourDef_Copy_impl_0046133f(CArmourDef* thisptr, CArmourDef* other)
{
    (void)thisptr; (void)other;
    g_called = 1;
}

extern void __fastcall CArmourDef_Copy_0046133f(CArmourDef* thisptr, CArmourDef* other);

int main()
{
    CArmourDef* a = (CArmourDef*)0x1000;
    CArmourDef* b = (CArmourDef*)0x2000;
    CArmourDef_Copy_0046133f(a, b);
    if (g_called == 1)
        printf("PASS_0046133f_OK\n");
    else
        printf("FAIL_0046133f\n");
    return 0;
}