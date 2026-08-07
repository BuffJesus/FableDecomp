#include "rebuild_abi.h"
#include <cstdio>
struct C { char p[0x368]; unsigned f368; };
extern "C" int FABLE_FASTCALL FableRet_c60fe0(C *self){ self->f368 |= 0x80000000u; return 0; }
int main(){ C o; o.f368=1; if(FableRet_c60fe0(&o)!=0){std::printf("B1");return 1;} if(o.f368!=0x80000001u){std::printf("B2");return 1;} std::printf("RB_c60fe0_OK\n"); return 0; }