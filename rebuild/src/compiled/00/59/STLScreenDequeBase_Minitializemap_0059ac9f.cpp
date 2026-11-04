#pragma optimize("s", on)
#include "fable_nui_manager.h"
namespace _STL {
template<class T,class A> void _Deque_base<T,A>::_M_initialize_map(size_t n) {
 const size_t buffer=sizeof(T)<128?128/sizeof(T):1;
 size_t nodes=n/buffer+1;
 _M_map_size=(_STL::max)(size_t(8),nodes+2);
 _M_map=_M_map_size?static_cast<T**>(malloc(_M_map_size*sizeof(T*))):0;
 T** start=_M_map+(_M_map_size-nodes)/2;
 T** finish=start+nodes;
 _M_create_nodes(start,finish);
 _M_start._M_set_node(start);_M_finish._M_set_node(finish-1);
 _M_start._M_cur=_M_start._M_first;
 _M_finish._M_cur=_M_finish._M_first+n%buffer;
}
template void _Deque_base<NUISystem::CComponent*>::_M_initialize_map(size_t);
}