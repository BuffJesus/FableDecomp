#include "rebuild_abi.h"
#include <stdio.h>

extern void __fastcall sub_00414db0(void* ecx_first, void* edx_last, void* unused_stack);

static int g_calls = 0;
static void* g_last = 0;

/* stand-in for the masked callee; the lander links the test against this. */
void __fastcall elem_op_00589d30(void* elem)
{
    g_calls++;
    g_last = elem;
}

int main(void)
{
    void* arr[4];
    arr[0] = (void*)1; arr[1] = (void*)2; arr[2] = (void*)3; arr[3] = (void*)4;

    /* empty range: no calls */
    sub_00414db0(&arr[0], &arr[0], (void*)0);
    if (g_calls != 0) { printf("BAD\n"); return 1; }

    /* full range of 4 slots */
    sub_00414db0(&arr[0], &arr[4], (void*)0);
    if (g_calls == 4 && g_last == (void*)&arr[3])
        printf("OK_414db0_A7C3\n");
    else
        printf("BAD\n");
    return 0;
}