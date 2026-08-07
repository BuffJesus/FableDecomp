#include "rebuild_abi.h"
#include <cstdio>
struct A { int x; }; struct B { char pad[0xc]; void *member; };
static A g_a_5edcf0; static B g_b_5edcf0; static void *g_dtored_5edcf0=0;
extern "C" A * FABLE_CDECL FableGetA_5edcf0(){ return &g_a_5edcf0; }
extern "C" B * FABLE_FASTCALL FableGetB_5edcf0(A *a){ g_b_5edcf0.member=(void*)0x77; return &g_b_5edcf0; }
extern "C" void FABLE_FASTCALL FableDtorC_5edcf0(void *c){ g_dtored_5edcf0=c; }
extern "C" void FABLE_CDECL FableDestvalChain_5edcf0(){ B *b=FableGetB_5edcf0(FableGetA_5edcf0()); FableDtorC_5edcf0(*(void**)((char*)b + 0xc)); }
int main(){
    FableDestvalChain_5edcf0();
    if(g_dtored_5edcf0!=(void*)0x77){std::printf("FAIL\n");return 1;}
    std::printf("DVCHAIN_OK\n"); return 0;
}