#include "fable_game_text_bank.h"
typedef _STL::set<CBankFileAsyncData*>::_Rep_type Tree;
typedef _STL::_Rb_tree_node<CBankFileAsyncData*> Node;
template<> void Tree::_M_erase(Node* node) {
    while(node) {
        _M_erase(static_cast<Node*>(node->_M_right));
        Node* left=static_cast<Node*>(node->_M_left);
        free(node);node=left;
    }
}
