#include "rebuild_abi.h"
#include <cstdio>
static void* rv=0;
extern "C" void* FABLE_FASTCALL Task_94b330_A(void* arg){ return rv; }
extern "C" bool FABLE_FASTCALL Is_94b330(void* self, void* arg){ void* r=Task_94b330_A(arg); return self==r; }
int main(){ int s,a; rv=&s; if(Is_94b330(&s,&a)!=1){std::printf("B1");return 1;} rv=&a; if(Is_94b330(&s,&a)!=0){std::printf("B2");return 1;}
 std::printf("RB_94b330_OK\n"); return 0; }