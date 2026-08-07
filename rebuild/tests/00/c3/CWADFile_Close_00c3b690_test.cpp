#include "rebuild_abi.h"
#include <cstdio>
struct C { void* f0; int f4; int f8; };
extern void* g_c3b690_ptr;
extern "C" C* FABLE_FASTCALL FableRet_c3b690(C* self){
    self->f0 = g_c3b690_ptr;
    self->f4 = 0;
    self->f8 = 0;
    return self;
}
static int marker = 0;
static int* markerptr = &(marker);
void* g_c3b690_ptr = markerptr;
int main(){ C o; o.f4=9; o.f8=9;
 if(FableRet_c3b690(&o)!=&o){std::printf("B1");return 1;}
 if(o.f0!=(void*)markerptr||o.f4!=0||o.f8!=0){std::printf("B2");return 1;}
 std::printf("RB_c3b690_OK\n"); return 0; }