#include "rebuild_abi.h"
// CEngineComponent::~CEngineComponent @ 0x00b59730
// mov [ecx],vtbl; ecx=[ecx+4]; if(ecx) [ecx]=0;
extern void *FableVtbl_CEngineComponent_b59730[];
struct CEngineComponent { void *vptr; void **m_owner; ~CEngineComponent(); };
CEngineComponent::~CEngineComponent()
{
    vptr = (void*)&FableVtbl_CEngineComponent_b59730;
    if (m_owner)
        *m_owner = 0;
}