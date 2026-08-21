#include <float.h>
extern "C" void __cdecl __setdefaultprecision()
{
    _controlfp(0x10000, 0x30000);
}
