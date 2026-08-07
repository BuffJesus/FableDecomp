#include "rebuild_abi.h"
#include <cstdio>
struct C { void Helper(void* b); };
static void* seen=0;
void C::Helper(void* b){ seen=b; }
extern "C" void FABLE_STDCALL New_c35af0(C* a, void* b){ a->Helper(b); }
int main(){ C o; int v; New_c35af0(&o,&v); if(seen!=&v){std::printf("B1");return 1;} std::printf("RB_c35af0_OK\n"); return 0; }