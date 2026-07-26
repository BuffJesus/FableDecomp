#include "rebuild_abi.h"
#include <cstdio>

struct COpinionPersonalityDef;

extern "C" int printf(const char*, ...);

/* Provide the impl so the test links; capture that this+src forward through. */
static COpinionPersonalityDef* g_this = 0;
static COpinionPersonalityDef* g_src  = 0;

void __fastcall COpinionPersonalityDef_Copy_impl(
    COpinionPersonalityDef* thisptr, void* /*edx*/, COpinionPersonalityDef* src)
{
    g_this = thisptr;
    g_src  = src;
}

void __fastcall COpinionPersonalityDef_Copy(
    COpinionPersonalityDef* thisptr, void* edx, COpinionPersonalityDef* src);

int main(void)
{
    COpinionPersonalityDef* a = (COpinionPersonalityDef*)0x1000;
    COpinionPersonalityDef* b = (COpinionPersonalityDef*)0x2000;
    COpinionPersonalityDef_Copy(a, 0, b);
    if (g_this == a && g_src == b)
        printf("OK_045e65b\n");
    else
        printf("BAD\n");
    return 0;
}