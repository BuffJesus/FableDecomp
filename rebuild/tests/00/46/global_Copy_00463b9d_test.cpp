#include "rebuild_abi.h"
#include <cstdio>

struct CEntitySoundDef;

// Provide a definition for the tail-call target so the test links & runs.
static int g_called = 0;
extern "C" void mark_called() { g_called = 1; }

extern void __fastcall CEntitySoundDef_Copy_impl(CEntitySoundDef* thisptr, CEntitySoundDef* src);
void __fastcall CEntitySoundDef_Copy_impl(CEntitySoundDef* thisptr, CEntitySoundDef* src)
{
    (void)thisptr; (void)src;
    mark_called();
}

extern void __fastcall CEntitySoundDef_Copy(CEntitySoundDef* thisptr, CEntitySoundDef* src);

int main()
{
    CEntitySoundDef* a = (CEntitySoundDef*)0x1000;
    CEntitySoundDef* b = (CEntitySoundDef*)0x2000;
    CEntitySoundDef_Copy(a, b);
    if (g_called == 1)
        printf("OK_463b9d\n");
    else
        printf("FAIL_463b9d\n");
    return 0;
}