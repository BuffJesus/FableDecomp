#include "rebuild_abi.h"
#include <cstdio>
struct Vec8 { void *m_begin; void *m_end; };
struct GroupChunk8 { char pad[0x4]; Vec8 v; void *GetList(); };
void *GroupChunk8::GetList(){ Vec8 *pv=&v; if((unsigned)(((char*)pv->m_end-(char*)pv->m_begin)>>3)>0) return pv->m_begin; return pv; }
int main(){ GroupChunk8 o; char buf[64]; o.v.m_begin=buf; o.v.m_end=buf+64;
 if(o.GetList()!=buf){std::printf("B1\n");return 1;}
 o.v.m_end=buf; if(o.GetList()!=&o.v){std::printf("B2\n");return 1;}
 std::printf("GL_ac40a0_OK\n"); return 0; }