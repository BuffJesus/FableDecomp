#include <stdio.h>
#include <string.h>
#include <new>
#include "fable_nui_manager.h"
static int calls;static void* receiver;static size_t amount=99;
namespace _STL {template<class T,class A> _Deque_base<T,A>::_Deque_base(const A&,size_t n){++calls;receiver=this;amount=n;memset(this,0x72,sizeof(*this));}template _Deque_base<NUISystem::CComponent*>::_Deque_base(const allocator<NUISystem::CComponent*>&,size_t);}
int main(){typedef _STL::stack<NUISystem::CComponent*> S;union{double align;unsigned char b[48];}u;memset(u.b,0xa5,48);S* p=new(u.b+4)S;if(calls!=1||receiver!=p||amount!=0||sizeof(*p)!=40)return 1;for(int i=0;i<40;++i)if(u.b[4+i]!=0x72)return 2;for(int j=0;j<4;++j)if(u.b[j]!=0xa5||u.b[j+44]!=0xa5)return 3;puts("SCREEN_STACK_CTOR_PASS");return 0;}
