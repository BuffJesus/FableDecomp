#include "rebuild_abi.h"
#include <stdio.h>

struct CCombatSequenceDef { int x; };

// Provide the (normally extern) tail-call target for the local test build.
static int g_called = 0;
static void* g_seen_this = 0;
static void* g_seen_other = 0;

void __fastcall CCombatSequenceDef_Copy_impl(CCombatSequenceDef* thisptr, void* other)
{
    g_called = 1;
    g_seen_this = (void*)thisptr;
    g_seen_other = other;
}

void __fastcall CCombatSequenceDef_Copy(CCombatSequenceDef* thisptr, void* other);

int main(void)
{
    CCombatSequenceDef obj;
    obj.x = 0x1234;
    int marker = 0;
    CCombatSequenceDef_Copy(&obj, &marker);

    if (g_called == 1 &&
        g_seen_this == (void*)&obj &&
        g_seen_other == (void*)&marker)
    {
        printf("OK_461d4b\n");
    }
    else
    {
        printf("FAIL_461d4b\n");
    }
    return 0;
}