#include "rebuild_abi.h"
#include <stdio.h>

struct CGlobalDef { int dummy; };

// Provide the tail-call target so the test links.
static int g_called = 0;
static CGlobalDef* g_this = 0;
static CGlobalDef* g_other = 0;

extern "C" {}

void __fastcall CGlobalDef_Copy_impl(CGlobalDef* thisptr, CGlobalDef* other)
{
    g_called = 1;
    g_this = thisptr;
    g_other = other;
}

void __fastcall CGlobalDef_Copy(CGlobalDef* thisptr, CGlobalDef* other);

int main()
{
    CGlobalDef a; a.dummy = 1;
    CGlobalDef b; b.dummy = 2;
    CGlobalDef_Copy(&a, &b);
    if (g_called == 1 && g_this == &a && g_other == &b)
        printf("OK_465eae\n");
    else
        printf("FAIL_465eae\n");
    return 0;
}