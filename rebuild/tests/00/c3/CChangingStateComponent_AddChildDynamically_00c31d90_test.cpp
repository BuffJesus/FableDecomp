#include "rebuild_abi.h"
#include <cstdio>
static int g_called=0;
struct CChild { virtual void v0(){} virtual void v1(){} virtual void v2(){} virtual void addChild(){ g_called=1; } };
struct CChangingStateComponent { CChild *first; void AddChildDynamically(); };
void CChangingStateComponent::AddChildDynamically(){ CChild *c=first; if(c) c->addChild(); }
int main(){
    CChangingStateComponent h; h.first=0; h.AddChildDynamically();
    if(g_called){std::printf("BADNULL\n");return 1;}
    CChild ch; h.first=&ch; h.AddChildDynamically();
    if(!g_called){std::printf("BADCALL\n");return 1;}
    std::printf("ADDCHILD_OK\n"); return 0;
}