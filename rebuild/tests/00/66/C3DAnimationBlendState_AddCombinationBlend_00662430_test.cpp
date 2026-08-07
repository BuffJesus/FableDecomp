#include "rebuild_abi.h"
#include <cstdio>
static int g_added=0;
struct C3DAnimationBlendState;
struct Sub { void Add(C3DAnimationBlendState *self); };
void Sub::Add(C3DAnimationBlendState *self){ g_added=1; }
struct Obj { char pad[0x4c]; Sub *m_sub; };
static Sub g_sub; static Obj g_obj;
extern "C" Obj *FABLE_FASTCALL FableGet_662430(C3DAnimationBlendState *self){ g_obj.m_sub=&g_sub; return &g_obj; }
struct C3DAnimationBlendState { char dummy; void AddCombinationBlend(); };
void C3DAnimationBlendState::AddCombinationBlend(){ Sub *s=FableGet_662430(this)->m_sub; s->Add(this); }
int main(){ C3DAnimationBlendState o; o.AddCombinationBlend();
 if(!g_added){std::printf("B1\n");return 1;}
 std::printf("AC_662430_OK\n"); return 0; }