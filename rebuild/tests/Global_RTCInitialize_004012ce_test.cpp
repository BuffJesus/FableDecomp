#include <stdio.h>

typedef void (__cdecl* RtcInitializer)();
extern "C" void __cdecl __RTC_Initialize();
extern "C" RtcInitializer DAT_01368e58[64] = { 0 };
extern "C" RtcInitializer DAT_01368f58[1] = { 0 };
static int g_calls;
static void __cdecl First() { g_calls = g_calls * 10 + 1; }
static void __cdecl Second() { g_calls = g_calls * 10 + 2; }

int main()
{
    if (DAT_01368f58 - DAT_01368e58 != 64) return 1;
    DAT_01368e58[0] = &First;
    DAT_01368e58[7] = &Second;
    DAT_01368e58[63] = &First;
    __RTC_Initialize();
    if (g_calls != 121) return 2;
    printf("RTC_INITIALIZE_TEST PASS\n");
    return 0;
}
