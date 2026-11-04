#include "fable_retail_banks.h"
#include "fable_stlport_tree.h"

NBankFileManager::CContainedBankMap::iterator
NBankFileManager::CContainedBankMap::InsertNode(
    CNode* position, CNode* parent, const value_type& value, fable_u32 forceRight)
{
    CNode* node;
    if (parent != head_ &&
        (forceRight || (position == 0 && !(value.first < parent->value_.first))))
    {
        node = CreateNode(value);
        parent->right_ = node;
        if (parent == head_->right_)
            head_->right_ = node;
    }
    else
    {
        node = CreateNode(value);
        parent->left_ = node;
        if (parent == head_)
        {
            head_->parent_ = node;
            head_->right_ = node;
        }
        else if (parent == head_->left_)
            head_->left_ = node;
    }
    node->parent_ = parent;
    node->left_ = 0;
    node->right_ = 0;
    _STL::_Rb_global<bool>::_Rebalance(
        reinterpret_cast<_STL::_Rb_tree_node_base*>(node),
        reinterpret_cast<_STL::_Rb_tree_node_base*&>(head_->parent_));
    ++size_;
    return iterator(node);
}
