#include "rebuild_abi.h"
#include <stdio.h>

struct CDefClassBase;
struct CSpecialAbilitiesMultiStrikeDef;

// Capture the tail-call so the test is deterministic without the real engine impl.
static int g_called = 0;
static void* g_this = 0;
static const void* g_src = 0;

extern void __fastcall CSpecialAbilitiesMultiStrikeDef_Copy_impl(
    CSpecialAbilitiesMultiStrikeDef* thisptr, const CDefClassBase* src)
{
    g_called = 1;
    g_this = (void*)thisptr;
    g_src = (const void*)src;
}

void __fastcall CSpecialAbilitiesMultiStrikeDef_Copy(
    CSpecialAbilitiesMultiStrikeDef* thisptr, const CDefClassBase* src);

int main(void)
{
    char obj[8];
    char src[8];
    CSpecialAbilitiesMultiStrikeDef_Copy(
        (CSpecialAbilitiesMultiStrikeDef*)obj, (const CDefClassBase*)src);
    if (g_called == 1 && g_this == (void*)obj && g_src == (const void*)src) {
        printf("OK_0045f711\n");
    } else {
        printf("FAIL_0045f711\n");
    }
    return 0;
}