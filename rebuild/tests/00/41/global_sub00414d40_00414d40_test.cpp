#include "rebuild_abi.h"
#include <cstdio>

static int g_calls = 0;
static void* g_last = 0;

/* Provide the extern callee so the test links standalone. */
void __fastcall elem_callback_00589da0(void* thisptr)
{
    g_calls++;
    g_last = thisptr;
}

void __fastcall sub_00414d40(void* first, void* last, void* unused);

int main(void)
{
    int buf[5];
    sub_00414d40(&buf[0], &buf[5], (void*)0);
    /* Expect 5 calls, last element = &buf[4]. */
    if (g_calls == 5 && g_last == (void*)&buf[4])
        printf("PASS_00414d40_OK\n");
    else
        printf("FAIL_00414d40 calls=%d\n", g_calls);
    return 0;
}