#include "rebuild_abi.h"
#include <cstdio>

struct CDefClassBase;
struct CSpecialAbilitiesMultiArrowDef;

extern void __fastcall CSpecialAbilitiesMultiArrowDef_Copy(
    CSpecialAbilitiesMultiArrowDef* thisptr, void* edx, CDefClassBase* other);

// Provide the extern impl target for linking the test.
void __fastcall CSpecialAbilitiesMultiArrowDef_Copy_impl(
    CSpecialAbilitiesMultiArrowDef* thisptr, void* edx, CDefClassBase* other)
{
    (void)thisptr; (void)edx; (void)other;
}

int main()
{
    CSpecialAbilitiesMultiArrowDef_Copy(
        (CSpecialAbilitiesMultiArrowDef*)0, (void*)0, (CDefClassBase*)0);
    printf("OK_45F7C2\n");
    return 0;
}