#include "fable_retail_banks.h"
#include "fable_stlport_tree.h"

std::pair<CBasicRetailBankAliasMap::iterator, bool>
CBasicRetailBankAliasMap::InsertUnique(const value_type& value)
{
    const _STL::less<CCharString>& less =
        *reinterpret_cast<const _STL::less<CCharString>*>(&compare_);
    CNode* parent = head_;
    CNode* node = head_->parent_;
    bool before = true;
    while (node != 0)
    {
        parent = node;
        before = less(value.first, node->value_.first);
        node = before ? node->left_ : node->right_;
    }
    iterator where(parent);
    if (before)
        if (where.node_ == head_->left_)
            return std::pair<iterator, bool>(InsertNode(parent, parent, value, 0), true);
        else
            --where;
    if (less(where.node_->value_.first, value.first))
        return std::pair<iterator, bool>(InsertNode(node, parent, value, 0), true);
    return std::pair<iterator, bool>(where, false);
}
