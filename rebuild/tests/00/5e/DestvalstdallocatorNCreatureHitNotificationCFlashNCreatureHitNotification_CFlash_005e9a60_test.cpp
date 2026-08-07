#include "rebuild_abi.h"
#include <cstdio>
struct C { void Helper(int n); };
static int hit=0;
void C::Helper(int n){ hit=n; }
extern "C" void FABLE_STDCALL Dest_5e9a60(C* p){ p->Helper(1); }
int main(){ C o; Dest_5e9a60(&o); if(hit!=1){std::printf("B1");return 1;} std::printf("RB_5e9a60_OK\n"); return 0; }