#include "rebuild_abi.h"
struct C { char p[0x368]; unsigned f368; };
extern "C" int FABLE_FASTCALL FableRet_c60fe0(C *self){ self->f368 |= 0x80000000u; return 0; }