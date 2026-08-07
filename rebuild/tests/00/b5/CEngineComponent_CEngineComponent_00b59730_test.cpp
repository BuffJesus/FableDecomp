#include "rebuild_abi.h"
#include <cstdio>
static void *g_vtbl[4];
void *FableVtbl_CEngineComponent_b59730[4];
struct CEngineComponent { void *vptr; void **m_owner; ~CEngineComponent(); };
CEngineComponent::~CEngineComponent(){ vptr=(void*)&FableVtbl_CEngineComponent_b59730; if(m_owner) *m_owner=0; }
int main(){ CEngineComponent o; void *owner=(void*)1; o.m_owner=&owner; o.~CEngineComponent();
 if(owner!=0){std::printf("B1\n");return 1;}
 std::printf("EC_b59730_OK\n"); return 0; }