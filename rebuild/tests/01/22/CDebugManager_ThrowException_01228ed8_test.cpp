#include "rebuild_abi.h"
#include <cstdio>
static int g_hit=0; static void *g_a=0,*g_d=0; static int g_b=0,g_c=0;
extern "C" void FABLE_STDCALL FableThrow_1228ed8(void *a, int b, int c, void *d){ g_hit=1; g_a=a; g_b=b; g_c=c; g_d=d; }
void FABLE_CDECL FableThrowException_1228ed8(){ FableThrow_1228ed8((void*)0x13b8b10,4,0xa,(void*)0x99eae0); }
int main(){ FableThrowException_1228ed8();
 if(!g_hit||g_b!=4||g_c!=0xa){std::printf("B1\n");return 1;}
 std::printf("TE_1228ed8_OK\n"); return 0; }