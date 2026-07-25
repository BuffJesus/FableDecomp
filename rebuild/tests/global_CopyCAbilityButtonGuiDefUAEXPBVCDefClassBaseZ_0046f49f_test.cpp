#include <cstdio>
#include "rebuild_abi.h"

struct CDefClassBase;
struct CAbilityButtonGuiDef;

extern "C" CAbilityButtonGuiDef* __fastcall
CAbilityButtonGuiDef_Copy_impl_0046f49f(CAbilityButtonGuiDef* thisptr, const CDefClassBase* src)
{
    // Test stub: identity-ish, echoes this pointer.
    (void)src;
    return thisptr;
}

CAbilityButtonGuiDef* __fastcall
CAbilityButtonGuiDef_Copy_0046f49f(CAbilityButtonGuiDef* thisptr, const CDefClassBase* src);

int main()
{
    CAbilityButtonGuiDef* self = (CAbilityButtonGuiDef*)0x1234;
    const CDefClassBase* src = (const CDefClassBase*)0x5678;
    CAbilityButtonGuiDef* r = CAbilityButtonGuiDef_Copy_0046f49f(self, src);
    if (r == self)
        printf("PASS_0046f49f\n");
    else
        printf("FAIL_0046f49f\n");
    return 0;
}