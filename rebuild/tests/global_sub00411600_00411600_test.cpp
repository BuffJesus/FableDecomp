#include <stdio.h>

/* Provide the callee locally for the test build. It counts how many
   times it was invoked and sums the payloads of the elements. */
static int g_calls = 0;
static int g_sum = 0;

void __fastcall sub_00589f10(void* thisptr)
{
    g_calls += 1;
    g_sum += *(int*)thisptr;
}

extern void __fastcall sub_00411600(void* ecx, void* edx, int unused);

int main(void)
{
    int arr[4];
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;
    arr[3] = 40;

    sub_00411600((void*)&arr[0], (void*)&arr[4], 0);

    if (g_calls == 4 && g_sum == 100)
        printf("PASS_00411600_OK\n");
    else
        printf("FAIL_00411600 calls=%d sum=%d\n", g_calls, g_sum);
    return 0;
}