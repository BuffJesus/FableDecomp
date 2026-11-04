#include <stdio.h>
#include <string.h>
#include "fable_nui_manager.h"
static int calls;static size_t made;static void* receiver;
namespace _STL {
template<class T,class A> _Deque_base<T,A>::_Deque_base(const A&,size_t){}
template<class T,class A> void _Deque_base<T,A>::_M_create_nodes(T** first,T** last){++calls;receiver=this;made=last-first;while(first!=last){*first=static_cast<T*>(malloc(128));memset(*first,0x73,128);++first;}}
template void _Deque_base<NUISystem::CComponent*>::_M_create_nodes(NUISystem::CComponent***,NUISystem::CComponent***);
}
typedef _STL::_Deque_base<NUISystem::CComponent*> B;
struct Access:B{Access():B(_STL::allocator<NUISystem::CComponent*>(),0){}using B::_M_start;using B::_M_finish;using B::_M_map;using B::_M_map_size;};
#define CHECK(x) do{if(!(x)){printf("FAIL %d n=%u\n",__LINE__,(unsigned)n);return 1;}}while(0)
int main(){size_t counts[]={0,1,31,32,33,159,160,191,192,193,1024};for(int k=0;k<11;++k){size_t n=counts[k],nodes=n/32+1;Access p;p._M_initialize_map(n);size_t capacity=nodes+2>8?nodes+2:8;CHECK(calls==k+1&&receiver==&p&&made==nodes);CHECK(p._M_map_size==capacity);CHECK(p._M_start._M_node==p._M_map+(capacity-nodes)/2);CHECK(p._M_finish._M_node==p._M_start._M_node+nodes-1);CHECK(p._M_start._M_cur==p._M_start._M_first&&p._M_start._M_first==*p._M_start._M_node);CHECK(p._M_finish._M_cur==*p._M_finish._M_node+n%32);CHECK(p._M_start._M_last==p._M_start._M_first+32&&p._M_finish._M_last==p._M_finish._M_first+32);for(size_t i=0;i<nodes;++i){unsigned char* b=(unsigned char*)p._M_start._M_node[i];for(int j=0;j<128;++j)CHECK(b[j]==0x73);free(b);}free(p._M_map);}puts("SCREEN_MAP_INIT_PASS");return 0;}