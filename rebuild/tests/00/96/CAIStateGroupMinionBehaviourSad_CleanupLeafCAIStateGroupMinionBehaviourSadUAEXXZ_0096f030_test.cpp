#include "rebuild_abi.h"
#include <cstdio>
struct Sub { void Helper(int n); };
static int hit=0;
void Sub::Helper(int n){ hit=1; }
struct C { char p24[0x24]; Sub* f24; char p28[0x3c-0x28]; char f3c; void Cleanup(); };
void C::Cleanup(){ this->f24->Helper(0x14); this->f3c=0; }
int main(){ C o; o.f3c=9; Sub s; o.f24=&s; o.Cleanup(); if(o.f3c!=0||!hit){std::printf("B1");return 1;} std::printf("RB_96f030_OK\n"); return 0; }