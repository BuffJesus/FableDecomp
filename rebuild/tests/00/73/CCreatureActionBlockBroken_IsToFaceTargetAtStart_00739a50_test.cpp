#include "rebuild_abi.h"
#include <cstdio>
struct C { char d; };
extern "C" char FABLE_FASTCALL FableRet_739a50(C *self){ return 1; }
int main(){ C o; if(FableRet_739a50(&o)!=1){std::printf("B1\n");return 1;}
 std::printf("RB_739a50_OK\n"); return 0; }