#include "rebuild_abi.h"
#include <cstdio>
struct S { char p[0x5c]; unsigned char f5c; };
struct C { char p[0x40]; S* f40; bool Can(int a); };
bool C::Can(int a){ return this->f40->f5c==0; }
int main(){ S s; s.f5c=0; C o; o.f40=&s; if(!o.Can(1)){std::printf("B1");return 1;} s.f5c=9; if(o.Can(1)){std::printf("B2");return 1;} std::printf("RB_bfddb0_OK\n"); return 0; }