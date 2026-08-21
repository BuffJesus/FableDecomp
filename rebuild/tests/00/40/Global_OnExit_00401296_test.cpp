#include <stdlib.h>
#include <stdio.h>

// Links against the real reconstruction. The source tail-jumps through an IMPORT slot,
// so the test has to supply that slot: a C symbol spelled `_imp__CrtOnExit` is emitted
// as `__imp__CrtOnExit`, which is exactly the name `__declspec(dllimport) CrtOnExit`
// resolves to. That lets BOTH branches run here (the old test could only reach one).

extern "C" _onexit_t __cdecl __onexit(_onexit_t function);
extern "C" _onexit_t* DAT_0143e990 = 0;
extern "C" _onexit_t* DAT_0143e9a0 = 0;

static int g_dllCalls;
static int g_crtCalls;
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

static _onexit_t __cdecl FakeCrtOnExit(_onexit_t function)
{
    ++g_crtCalls;
    return function;
}

extern "C" _onexit_t (__cdecl *_imp__CrtOnExit)(_onexit_t) = &FakeCrtOnExit;

int main()
{
    DAT_0143e9a0 = 0;
    __onexit(&Callback);
    if (g_dllCalls != 1 || g_seenFunction != &Callback) return 1;
    if (g_seenFirst != &DAT_0143e9a0 || g_seenLast != &DAT_0143e990) return 2;

    DAT_0143e9a0 = reinterpret_cast<_onexit_t*>(-1);
    if (__onexit(&Callback) != &Callback) return 3;
    if (g_dllCalls != 1 || g_crtCalls != 1) return 4;

    printf("GLOBAL_ONEXIT_TEST PASS\n");
    return 0;
}
