#pragma once
#include "fable_stlport_list.h"
namespace _STL {
template<class T,class A>
typename list<T,A>::iterator list<T,A>::erase(iterator position)
{
    _Node* node = static_cast<_Node*>(position._M_node);
    iterator next(node->_M_next);
    node->_M_unhook();
    _Destroy(&node->_M_data);
    allocator<_Node>().deallocate(node,1);
    return next;
}
template<class T,class A> template<class P>
void list<T,A>::remove_if(P predicate)
{
    iterator first = begin();
    iterator last = end();
    while (first != last) {
        iterator next = first;
        ++next;
        if (predicate(*first)) erase(first);
        first = next;
    }
}
}
