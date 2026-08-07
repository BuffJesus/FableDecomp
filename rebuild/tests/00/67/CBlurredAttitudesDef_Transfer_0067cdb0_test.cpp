#include "rebuild_abi.h"
#include <cstdio>
static void *g_sub=0;
struct Target { void Xfer(void *sub); };
void Target::Xfer(void *sub){ g_sub=sub; }
struct CBlurredAttitudesDef { char pad[0x3c]; char sub[4]; void Transfer(Target *t); };
void CBlurredAttitudesDef::Transfer(Target *t){ t->Xfer((void*)&sub[0]); }
int main(){ CBlurredAttitudesDef o; Target t; o.Transfer(&t);
 if(g_sub!=(void*)&o.sub[0]){std::printf("B1\n");return 1;}
 std::printf("XF_67cdb0_OK\n"); return 0; }