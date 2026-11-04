#include <stdio.h>
#include <string.h>
#include "fable_nui_manager.h"
namespace _STL {template<class T,class A> _Deque_base<T,A>::_Deque_base(const A&,size_t){}}
int main(){typedef NUISystem::CComponent* P;_STL::_Deque_base<P> base(_STL::allocator<P>(),0);for(int n=0;n<=7;++n){P* slots[9]={0};P* guard=(P*)0x1234;slots[0]=slots[n+1]=guard;base._M_create_nodes(slots+1,slots+1+n);if(slots[0]!=guard||slots[n+1]!=guard)return 1;for(int i=1;i<=n;++i){if(!slots[i])return 2;for(int j=1;j<i;++j)if(slots[i]==slots[j])return 3;memset(slots[i],0x73,128);}for(int k=1;k<=n;++k)free(slots[k]);}puts("SCREEN_NODES_PASS");return 0;}
