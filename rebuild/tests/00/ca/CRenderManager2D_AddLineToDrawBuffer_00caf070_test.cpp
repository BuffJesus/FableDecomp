#include "rebuild_abi.h"
#include <cstdio>
static void* seen=(void*)-1;
extern "C" void FABLE_FASTCALL Helper_caf070(void* self){ seen=self; }
struct C { int x; };
extern "C" void FABLE_FASTCALL Handle_caf070(C* self){ if(self){ Helper_caf070((char*)self+4); } else { Helper_caf070(0); } }
int main(){ C o; Handle_caf070(&o); if(seen!=(char*)&o+4){std::printf("B1");return 1;} Handle_caf070(0); if(seen!=0){std::printf("B2");return 1;} std::printf("RB_caf070_OK\n"); return 0; }