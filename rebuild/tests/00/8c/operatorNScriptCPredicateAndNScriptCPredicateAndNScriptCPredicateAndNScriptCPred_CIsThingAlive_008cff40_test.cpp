#include "rebuild_abi.h"
#include <cstdio>
static int g_hit=0; static void *g_self=0;
struct Pred;
struct Sub { void And(Pred *self); };
void Sub::And(Pred *self){ g_hit=1; g_self=self; }
struct GObj { char pad[4]; Sub *m_sub; };
static Sub g_sub; static GObj g_obj;
GObj *g_pred_8cff40=&g_obj;
struct Pred { char d; Pred *Op(); };
Pred *Pred::Op(){ g_pred_8cff40->m_sub->And(this); return this; }
int main(){ g_obj.m_sub=&g_sub; Pred p; if(p.Op()!=&p){std::printf("B1\n");return 1;}
 if(!g_hit||g_self!=&p){std::printf("B2\n");return 1;}
 std::printf("AND_8cff40_OK\n"); return 0; }