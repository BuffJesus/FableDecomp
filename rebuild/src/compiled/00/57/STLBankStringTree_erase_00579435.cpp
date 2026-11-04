#pragma optimize("s", on)
#include "fable_string.h"
#include "fable_stlport_tree.h"
typedef _STL::map<CCharString, unsigned long>::_Rep_type BankStringTree;
typedef _STL::map<CCharString, unsigned long>::value_type BankStringValue;
typedef _STL::_Rb_tree_node<BankStringValue> BankStringNode;
template<> void BankStringTree::_M_erase(BankStringNode* node)
{
    while (node) {
        _M_erase(static_cast<BankStringNode*>(node->_M_right));
        BankStringNode* next = static_cast<BankStringNode*>(node->_M_left);
        node->_M_value_field.~BankStringValue();
        free(node);
        node = next;
    }
}
template void BankStringTree::_M_erase(BankStringNode*);
