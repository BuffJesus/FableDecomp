#include <new>
#include "fable_engine_manager.h"
namespace _STL {
template<class T,class A>
typename list<T,A>::iterator list<T,A>::insert(iterator position,const T& value)
{
    _Node* node=static_cast<_Node*>(malloc(sizeof(_Node)));
    new (&node->_M_data) T(value);
    _List_node_base* next=position._M_node;
    _List_node_base* previous=next->_M_prev;
    node->_M_next=next;node->_M_prev=previous;
    previous->_M_next=node;next->_M_prev=node;
    return iterator(node);
}
}
void CEngineManager::AddEngine(CIEngine& engine){Engines.push_back(&engine);}
void CEngineManager::RemoveEngine(CIEngine& engine){Engines.remove(&engine);}
