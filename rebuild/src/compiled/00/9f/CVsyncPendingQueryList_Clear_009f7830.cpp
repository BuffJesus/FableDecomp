#include "fable_display_viewport.h"
namespace _STL {
template<> void list<CVsyncCallbackTimer::CPendingQuery>::clear(){
 _Node* current=static_cast<_Node*>(_M_node->_M_next);
 while(current!=_M_node){
  _Node* next=static_cast<_Node*>(current->_M_next);
  _Destroy(&current->_M_data);
  if(current)free(current);
  current=next;
 }
 _M_node->_M_next=_M_node;_M_node->_M_prev=_M_node;
}
}
