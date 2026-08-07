#include "rebuild_abi.h"
// ConstructString @ 0x009c7b80
// mov eax,[esp+4]; push -1; push eax; push edx; push ecx; call helper; add esp,0x10; ret 4
// Forward (this, edx, arg, -1) to a __cdecl string constructor.
extern "C" void FABLE_CDECL FableStringCtor_9c7b80(void *self, void *edxv, void *arg, int len);
extern "C" void FABLE_FASTCALL FableConstructString_9c7b80(void *self, void *edxv, void *arg)
{
    FableStringCtor_9c7b80(self, edxv, arg, -1);
}