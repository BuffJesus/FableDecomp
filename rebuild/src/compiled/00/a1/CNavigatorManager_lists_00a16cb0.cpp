// STLport list destructors used by ??_GCNavigatorManager (retail 0x00A175A0):
//   0x00A16CB0  list<pair<long, const CNavigatorBase*> >::~list  (QueueManager.Navigators, +0x2C)
//   0x00A17570  list<CNavQuadTreeMap>::~list                     (QuadTrees, +0x24)
// The CNavQuadTreeMap clear stays out of line (0x00A17390).
#include "fable_navigator.h"

namespace _STL {
template<class T, class A> void list<T, A>::clear()
{
    _List_node_base* current = _M_node->_M_next;
    while (current != _M_node) {
        _Node* old = static_cast<_Node*>(current);
        current = current->_M_next;
        _Destroy(&old->_M_data);
        free(old);
    }
    _M_node->_M_next = _M_node;
    _M_node->_M_prev = _M_node;
}
template<class T, class A> list<T, A>::~list()
{
    clear();
    if (_M_node)
        free(_M_node);
}
template list<pair<long, const CNavigatorBase*> >::~list();
template list<CNavQuadTreeMap>::~list();
}
