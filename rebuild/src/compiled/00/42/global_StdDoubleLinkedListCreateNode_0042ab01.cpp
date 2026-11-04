#pragma optimize("s", on)
#include "fable_nui_manager.h"
namespace _STL {
template<class T,class A> list<T,A>::list(const A&) : _M_node(0) {
 _Node* p=static_cast<_Node*>(malloc(sizeof(_Node)));p->_M_next=p;p->_M_prev=p;_M_node=p;
}
template list<NUISystem::CActionParam*>::list(const allocator<NUISystem::CActionParam*>&);
}
