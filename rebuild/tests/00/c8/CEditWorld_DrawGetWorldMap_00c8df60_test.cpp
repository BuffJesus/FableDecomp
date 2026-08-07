#include "rebuild_abi.h"
#include <cstdio>
struct C { char p[0x1c]; int* f1c; };
extern "C" int FABLE_FASTCALL Get_c8df60(C* self, int i){ if(self->f1c) return self->f1c[i]; return -1; }
int main(){ int arr[3]={10,20,30}; C o; o.f1c=arr; if(Get_c8df60(&o,1)!=20){std::printf("B1");return 1;} o.f1c=0; if(Get_c8df60(&o,0)!=-1){std::printf("B2");return 1;} std::printf("RB_c8df60_OK\n"); return 0; }