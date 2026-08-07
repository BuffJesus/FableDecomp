#include "rebuild_abi.h"
#include <cstdio>
struct Pair24 { char raw[24]; };
static Pair24 *g_last=0;
struct ConsAlloc { void Helper(Pair24 *last); void ConsVal(unsigned n); };
void ConsAlloc::Helper(Pair24 *last){ g_last=last; }
void ConsAlloc::ConsVal(unsigned n){ Helper((Pair24*)this+(n+1)); }
int main(){ ConsAlloc a; a.ConsVal(2);
 if((char*)g_last-(char*)&a != 3*24){std::printf("B1 %ld\n",(long)((char*)g_last-(char*)&a));return 1;}
 std::printf("CN_b847b0_OK\n"); return 0; }