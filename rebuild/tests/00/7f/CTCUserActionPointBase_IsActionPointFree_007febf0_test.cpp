#include "rebuild_abi.h"
#include <cstdio>
struct C { char p[0xc]; void* fc; void* f10; bool Free(); };
bool C::Free(){ return this->fc != this->f10; }
int main(){ C o; int a,b; o.fc=&a; o.f10=&b; if(!o.Free()){std::printf("B1");return 1;} o.f10=&a; if(o.Free()){std::printf("B2");return 1;} std::printf("RB_7febf0_OK\n"); return 0; }