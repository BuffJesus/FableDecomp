#include "rebuild_abi.h"
#include <cstdio>
struct Sub { void Notify(void* a); }; static int hit=0;
void Sub::Notify(void* a){ hit=1; }
struct C { char p[0x1e0]; Sub s1e0; void Notify(void* a, void* b); };
void C::Notify(void* a, void* b){ this->s1e0.Notify(a); }
int main(){ C o; int x,y; o.Notify(&x,&y); if(!hit){std::printf("B1");return 1;} std::printf("RB_9ff8b0_OK\n"); return 0; }