#pragma optimize("s", on)
#include "fable_nui_manager.h"
namespace _STL {
template<class T,class A> _Deque_base<T,A>::_Deque_base(const A&,size_t n) : _M_map(0),_M_map_size(0) { _M_initialize_map(n); }
template _Deque_base<NUISystem::CComponent*>::_Deque_base(const allocator<NUISystem::CComponent*>&,size_t);
}