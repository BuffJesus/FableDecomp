#include "rebuild_abi.h"
// _atexit @ 0x007723d0
// mov eax,[esp+4]; push eax; call reg; neg eax; sbb eax,eax; neg eax; ret 4
typedef void (*FableAtexitFn_007723d0)();
int FABLE_STDCALL FableAtexit_reg_007723d0(FableAtexitFn_007723d0 fp);
extern "C" int FABLE_STDCALL FableAtexit_007723d0(FableAtexitFn_007723d0 fp)
{
    return FableAtexit_reg_007723d0(fp) != 0;
}