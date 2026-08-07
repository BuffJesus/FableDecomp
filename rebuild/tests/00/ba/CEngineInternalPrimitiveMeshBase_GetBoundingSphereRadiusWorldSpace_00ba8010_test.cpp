#include "rebuild_abi.h"
#include <cstdio>
struct SubObj { char pad[0x38]; float m_r; };
struct CEngineInternalPrimitiveMeshBase { char pad[0x50]; SubObj *m_sub; char gap[4]; float m_scale; bool GetR(float *out); };
bool CEngineInternalPrimitiveMeshBase::GetR(float *out){ *out=m_sub->m_r*m_scale; return true; }
int main(){ CEngineInternalPrimitiveMeshBase o; SubObj s; s.m_r=3.0f; o.m_sub=&s; o.m_scale=2.0f; float r=0;
 if(!o.GetR(&r)){std::printf("B1\n");return 1;} if(r!=6.0f){std::printf("B2\n");return 1;}
 std::printf("BS_ba8010_OK\n"); return 0; }