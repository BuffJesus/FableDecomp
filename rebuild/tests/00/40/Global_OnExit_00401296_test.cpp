#include <stdlib.h>
#include <stdio.h>

extern "C" void __cdecl __onexit(_onexit_t function);
extern "C" _onexit_t* DAT_0143e990 = 0;
extern "C" _onexit_t* DAT_0143e9a0 = 0;
static int g_dllCalls;
static _onexit_t g_seenFunction;
static _onexit_t** g_seenFirst;
static _onexit_t** g_seenLast;

static int __cdecl Callback() { return 0; }

extern "C" _onexit_t __cdecl __dllonexit(
    _onexit_t function, _onexit_t** first, _onexit_t** last)
{
    ++g_dllCalls; g_seenFunction = function; g_seenFirst = first; g_seenLast = last;
    return function;
}

int main()
{
    DAT_0143e9a0 = 0;
    __onexit(&Callback);
    if (g_dllCalls != 1 || g_seenFunction != &Callback) return 1;
    if (g_seenFirst != &DAT_0143e9a0 || g_seenLast != &DAT_0143e990) return 2;
    DAT_0143e9a0 = reinterpret_cast<_onexit_t*>(-1);
    __onexit(&Callback);
    if (g_dllCalls != 1) return 3;
    printf("GLOBAL_ONEXIT_TEST PASS\n");
    return 0;
}
