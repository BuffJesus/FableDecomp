#include <cstdio>

struct CPlayerMovementDef;

extern void __fastcall CPlayerMovementDef_Copy(CPlayerMovementDef* thisptr, CPlayerMovementDef* src);

// Provide the impl the thunk tail-calls so the test links & runs.
extern "C" int g_called_004628cb = 0;
void __fastcall CPlayerMovementDef_Copy_impl(CPlayerMovementDef* thisptr, CPlayerMovementDef* src)
{
    (void)thisptr; (void)src;
    g_called_004628cb = 1;
}

int main()
{
    CPlayerMovementDef_Copy((CPlayerMovementDef*)0x1000, (CPlayerMovementDef*)0x2000);
    if (g_called_004628cb == 1)
        printf("OK_THUNK_004628cb\n");
    else
        printf("FAIL_004628cb\n");
    return 0;
}