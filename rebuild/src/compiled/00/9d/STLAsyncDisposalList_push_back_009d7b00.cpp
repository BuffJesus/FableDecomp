#include <new>
#include "fable_game_text_bank.h"
template<class T> inline CCountedPointer<T>::CCountedPointer(const CCountedPointer& other)
    :Data(other.Data),Info(other.Info) { if(Info)++Info->RefCount; }
namespace _STL {
template<class T,class A> inline typename list<T,A>::iterator list<T,A>::insert(iterator position,const T& value) {
    _Node* node=static_cast<_Node*>(malloc(sizeof(_Node)));
    new (&node->_M_data) T(value);
    _List_node_base* next=position._M_node;
    _List_node_base* previous=next->_M_prev;
    node->_M_next=next;node->_M_prev=previous;
    previous->_M_next=node;next->_M_prev=node;
    return iterator(node);
}
template<> void list<CCountedPointer<CBankFileAsyncData> >::push_back(const CCountedPointer<CBankFileAsyncData>& value) {
    insert(end(),value);
}
template void list<CCountedPointer<CBankFileAsyncData> >::push_back(const CCountedPointer<CBankFileAsyncData>&);
}
