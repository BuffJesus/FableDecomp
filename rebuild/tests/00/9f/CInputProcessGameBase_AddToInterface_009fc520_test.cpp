#include "rebuild_abi.h"
#include <cstdio>
static void* seen=(void*)-1;
struct C { char p[0x28]; void* f28; void Add(void* a); void Helper(void* a); };
void C::Helper(void* a){ seen=a; }
void C::Add(void* a){ this->f28=a; this->Helper(0); }
int main(){ C o; int x; o.Add(&x); if(o.f28!=&x||seen!=0){std::printf("B1");return 1;} std::printf("RB_9fc520_OK\n"); return 0; }