#include "rebuild_abi.h"
#include <cstdio>
struct Vec32 { void *m_begin; void *m_end; };
struct C3DMeshFileGroupChunk { char pad[0x10]; Vec32 v; void *GetList(); };
void *C3DMeshFileGroupChunk::GetList(){ Vec32 *pv=&v; if((unsigned)(((char*)pv->m_end-(char*)pv->m_begin)>>5)>0) return pv->m_begin; return pv; }
int main(){ C3DMeshFileGroupChunk o; char buf[64]; o.v.m_begin=buf; o.v.m_end=buf+64;
 if(o.GetList()!=buf){std::printf("B1\n");return 1;}
 o.v.m_end=buf; if(o.GetList()!=&o.v){std::printf("B2\n");return 1;}
 std::printf("GL_abc420_OK\n"); return 0; }