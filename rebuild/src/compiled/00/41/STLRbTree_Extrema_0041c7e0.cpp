#include "fable_stlport_tree.h"
namespace _STL {
_Rb_tree_node_base* _Rb_tree_node_base::_S_minimum(_Rb_tree_node_base* node) {
    while(node->_M_left)node=node->_M_left;
    return node;
}
_Rb_tree_node_base* _Rb_tree_node_base::_S_maximum(_Rb_tree_node_base* node) {
    while(node->_M_right)node=node->_M_right;
    return node;
}
}
