#include "rebuild_abi.h"
#include <cstdio>
struct Sub { int Add(void* p); };
int Sub::Add(void* p){ return *(int*)p; }
struct C { char p44[0x44]; int f44; char p48[0x4c-0x48]; Sub s4c; int Learn(); };
int C::Learn(){ return this->s4c.Add(&this->f44); }
int main(){ C o; o.f44=99; if(o.Learn()!=99){std::printf("B1");return 1;} std::printf("RB_c36210_OK\n"); return 0; }