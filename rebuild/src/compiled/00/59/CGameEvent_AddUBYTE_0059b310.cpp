#pragma optimize("s", on)
#include "fable_nui_manager.h"
namespace _STL {
template<class T,class A> deque<T,A>::deque() : _Deque_base<T,A>(A(),0) {}
template deque<NUISystem::CComponent*>::deque();
}
