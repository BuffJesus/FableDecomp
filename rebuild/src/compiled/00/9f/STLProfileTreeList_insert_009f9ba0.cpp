#include <new>
#include "fable_system_manager.h"
namespace _STL {
 template<class T,class A> typename list<T,A>::iterator list<T,A>::insert(iterator position,const T& value){
  _Node* n=static_cast<_Node*>(malloc(sizeof(_Node)));
  _Construct(&n->_M_data,value);
  _List_node_base* next=position._M_node;_List_node_base* prev=next->_M_prev;
  n->_M_next=next;n->_M_prev=prev;prev->_M_next=n;next->_M_prev=n;
  return iterator(n);
 }
 template list<CProfileManager::CProfileTreeEntry>::iterator list<CProfileManager::CProfileTreeEntry>::insert(iterator,const CProfileManager::CProfileTreeEntry&);
}