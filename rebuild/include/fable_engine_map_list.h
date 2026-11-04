#pragma once
#include <new>
#include "engine/CEngineMap.h"
namespace _STL {
template<> inline list<CEngineMap*>::iterator list<CEngineMap*>::insert(iterator position,CEngineMap* const& value) {
    _Node* node=static_cast<_Node*>(malloc(sizeof(_Node)));
    new (&node->_M_data) CEngineMap*(value);
    _List_node_base* next=position._M_node;
    _List_node_base* previous=next->_M_prev;
    node->_M_next=next;node->_M_prev=previous;
    previous->_M_next=node;next->_M_prev=node;
    return iterator(node);
}
template<> inline list<CEngineMap*>::iterator list<CEngineMap*>::erase(iterator position) {
    iterator next(position);++next;
    position._M_node->_M_unhook();free(position._M_node);
    return next;
}
}
