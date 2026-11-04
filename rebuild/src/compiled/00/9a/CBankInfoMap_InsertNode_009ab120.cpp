#include "fable_retail_banks.h"
#include "fable_stlport_tree.h"
#include <new>

// Retail allocates tree nodes with malloc (BFEA0E), paired with Erase/free.
static __forceinline CBankFileManager::CBankInfoMap::CEntryNode*
CreateBankInfoNode(const CBankFileManager::CBankInfoMap::value_type& value)
{
    CBankFileManager::CBankInfoMap::CEntryNode* node =
        static_cast<CBankFileManager::CBankInfoMap::CEntryNode*>(
            malloc(sizeof(CBankFileManager::CBankInfoMap::CEntryNode)));
    void* valueAddress = &node->value_;
    if (valueAddress != 0)
        new (valueAddress) CBankFileManager::CBankInfoMap::value_type(value);
    return node;
}

CBankFileManager::CBankInfoMap::iterator
CBankFileManager::CBankInfoMap::InsertNode(
    CNode* position, CNode* parent, const value_type& value, fable_u32 forceRight)
{
    CEntryNode* node;
    if (parent != head_ &&
        (forceRight || (position == 0 && !(value.first < static_cast<CEntryNode*>(parent)->value_.first))))
    {
        node = CreateBankInfoNode(value);
        parent->right_ = node;
        if (parent == head_->right_)
            head_->right_ = node;
    }
    else
    {
        node = CreateBankInfoNode(value);
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
