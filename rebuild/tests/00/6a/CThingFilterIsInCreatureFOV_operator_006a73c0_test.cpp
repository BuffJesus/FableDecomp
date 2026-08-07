#include "rebuild_abi.h"
#include <cstdio>
static void *g_arg=0; static unsigned char g_flag=0xff;
struct Inner { void Call(void *arg, unsigned char flag); };
void Inner::Call(void *arg, unsigned char flag){ g_arg=arg; g_flag=flag; }
struct CThingFilter { Inner *m_inner; unsigned char m_flag; void operator()(void *arg); };
void CThingFilter::operator()(void *arg){ m_inner->Call(arg, m_flag); }
int main(){ Inner impl; CThingFilter f; f.m_inner=&impl; f.m_flag=1; int x; f(&x);
 if(g_arg!=&x||g_flag!=1){std::printf("B1\n");return 1;}
 std::printf("OP_6a73c0_OK\n"); return 0; }