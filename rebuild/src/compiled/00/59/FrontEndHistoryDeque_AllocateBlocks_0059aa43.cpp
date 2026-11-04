#pragma optimize("s", on)
#include "fable_nui_manager.h"
namespace _STL {
template<class T,class A> void _Deque_base<T,A>::_M_create_nodes(T** first,T** last) {
 for(T** p=first;p<last;++p)*p=static_cast<T*>(malloc(sizeof(T)*(sizeof(T)<128?128/sizeof(T):1)));
}
template void _Deque_base<NUISystem::CComponent*>::_M_create_nodes(NUISystem::CComponent***,NUISystem::CComponent***);
}
