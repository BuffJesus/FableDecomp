#include "fable_game_text_bank.h"
template<> void _STL::list<CCountedPointer<CBankFileAsyncData> >::clear() {
    _List_node_base* node=_M_node->_M_next;
    while(node!=_M_node) {
        _Node* current=static_cast<_Node*>(node);
        node=node->_M_next;
        _Destroy(&current->_M_data);
        free(current);
    }
    _M_node->_M_next=_M_node;
    _M_node->_M_prev=_M_node;
}
