#include "rebuild_abi.h"
#include <cstdio>
struct Sub1c { long count; };
extern "C" long FABLE_FASTCALL FableCount_c49620(Sub1c *s){ return s->count; }
struct CScriptModuleSelfTerminate { char pad1c[0x1c]; char m_sub[0x20]; long m_target; long IsFinished(); };
long CScriptModuleSelfTerminate::IsFinished(){ return m_target-FableCount_c49620((Sub1c*)&m_sub[0]); }
int main(){ CScriptModuleSelfTerminate o; ((Sub1c*)&o.m_sub[0])->count=5; o.m_target=8;
 if(o.IsFinished()!=3){std::printf("B1\n");return 1;}
 std::printf("IF_c49620_OK\n"); return 0; }