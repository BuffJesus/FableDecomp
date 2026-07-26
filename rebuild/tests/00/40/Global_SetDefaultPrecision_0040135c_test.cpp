#include <float.h>
#include <stdio.h>

extern "C" void __cdecl __setdefaultprecision();

int main()
{
    const unsigned int original = _controlfp(0, 0);
    _controlfp(_PC_24, _MCW_PC);
    __setdefaultprecision();
    const unsigned int changed = _controlfp(0, 0);
    _controlfp(original, _MCW_PC);
    if ((changed & _MCW_PC) != 0x10000) return 1;
    printf("SET_DEFAULT_PRECISION_TEST PASS\n");
    return 0;
}
