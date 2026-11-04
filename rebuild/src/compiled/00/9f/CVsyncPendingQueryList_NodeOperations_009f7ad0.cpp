#include "fable_display_viewport.h"
void* __cdecl operator new(size_t,void*) throw();
namespace _STL {
template<> __declspec(noinline) _List_iterator<CVsyncCallbackTimer::CPendingQuery>::_List_iterator(_List_node_base* node){_M_node=node;}
template<> list<CVsyncCallbackTimer::CPendingQuery>::_Node* list<CVsyncCallbackTimer::CPendingQuery>::_M_create_node(const CVsyncCallbackTimer::CPendingQuery& value){
 _Node* node=allocator<_Node>().allocate(1);
 new(&node->_M_data) CVsyncCallbackTimer::CPendingQuery(value);
 return node;
}
template<> __forceinline list<CVsyncCallbackTimer::CPendingQuery>::iterator list<CVsyncCallbackTimer::CPendingQuery>::insert(iterator position,const CVsyncCallbackTimer::CPendingQuery& value){
 _Node* node=_M_create_node(value);
 _List_node_base* following=position._M_node;
 _List_node_base* previous=following->_M_prev;
 node->_M_next=following;node->_M_prev=previous;previous->_M_next=node;following->_M_prev=node;
 return iterator(node);
}
template<> void list<CVsyncCallbackTimer::CPendingQuery>::push_front(const CVsyncCallbackTimer::CPendingQuery& value){insert(begin(),value);}
template<> void list<CVsyncCallbackTimer::CPendingQuery>::push_back(const CVsyncCallbackTimer::CPendingQuery& value){insert(end(),value);}
}

template void _STL::list<CVsyncCallbackTimer::CPendingQuery>::push_front(const CVsyncCallbackTimer::CPendingQuery&);
template void _STL::list<CVsyncCallbackTimer::CPendingQuery>::push_back(const CVsyncCallbackTimer::CPendingQuery&);

template _STL::list<CVsyncCallbackTimer::CPendingQuery>::iterator _STL::list<CVsyncCallbackTimer::CPendingQuery>::insert(iterator,const CVsyncCallbackTimer::CPendingQuery&);
