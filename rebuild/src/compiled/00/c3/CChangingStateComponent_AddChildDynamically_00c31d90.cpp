#include "rebuild_abi.h"
// CChangingStateComponent::AddChildDynamically @ 0x00c31d90
// mov ecx,[ecx]; test ecx,ecx; je ret; mov eax,[ecx]; jmp [eax+0xc]  -> if(this->f0) this->f0->vslot3()
struct CChild { virtual void v0(); virtual void v1(); virtual void v2(); virtual void addChild()=0; };
struct CChangingStateComponent { CChild *first; void AddChildDynamically(); };
void CChangingStateComponent::AddChildDynamically()
{
    CChild *c = first;
    if (c) c->addChild();
}