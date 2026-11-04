#include "fable_engine_manager.h"
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
template list<CIEngine*>::~list();
}
