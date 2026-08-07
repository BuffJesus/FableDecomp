#include "rebuild_abi.h"
#include <cstdio>
struct M { bool Check(); };
struct S { char p[0x1c]; void* f1c; };
static bool cv=false; bool M::Check(){ return cv; }
static M gm; static S gs;
extern "C" S* FABLE_FASTCALL GetS_946630(){ gs.f1c=&gm; return &gs; }
extern "C" bool FABLE_FASTCALL Should_946630(){ return !((M*)(GetS_946630()->f1c))->Check(); }
int main(){ cv=false; if(Should_946630()!=true){std::printf("B1");return 1;} cv=true; if(Should_946630()!=false){std::printf("B2");return 1;} std::printf("RB_946630_OK\n"); return 0; }