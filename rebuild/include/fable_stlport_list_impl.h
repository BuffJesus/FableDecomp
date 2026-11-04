#pragma once
// Out-of-line STLport list members that retail instantiates per element type:
// clear() and ~list(). In real STLport these bodies are _List_base::clear and
// ~_List_base; the project's single-class list (fable_stlport_list.h) names them
// list::clear / list::~list, as the accepted list<CIEngine*>::~list 0x00436900
// provider (EnginePointerList_Destructor_00436900.cpp) and the console/navigator
// list TUs already do with local copies of these same bodies.
// Include this header only in TUs that do not define these members themselves.
#include "fable_stlport_list.h"
namespace _STL {
template<class T,class A> void list<T,A>::clear()
{
    _List_node_base* current=_M_node->_M_next;
    while(current!=_M_node){
        _Node* old=static_cast<_Node*>(current);current=current->_M_next;
        _Destroy(&old->_M_data);free(old);
    }
    _M_node->_M_next=_M_node;_M_node->_M_prev=_M_node;
}
template<class T,class A> list<T,A>::~list()
{
    clear();
    if(_M_node)free(_M_node);
}
}
