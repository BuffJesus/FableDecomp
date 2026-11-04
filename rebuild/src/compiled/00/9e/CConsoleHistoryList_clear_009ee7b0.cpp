#include "fable_console.h"
namespace _STL {
 template<class T,class A> void list<T,A>::clear(){
  _List_node_base* cur=_M_node->_M_next;
  while(cur!=_M_node){
   _Node* old=static_cast<_Node*>(cur);cur=cur->_M_next;
   _Destroy(&old->_M_data);free(old);
  }
  _M_node->_M_next=_M_node;_M_node->_M_prev=_M_node;
 }
 template void list<CConsoleCommandLine>::clear();
}
