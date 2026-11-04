#include "fable_retail_banks.h"

CBasicRetailBankAliasMap::CNode*
CBasicRetailBankAliasMap::LowerBound(const CCharString& key) const
{
    CNode* candidate = head_;
    CNode* node = head_->parent_;
    while (node != 0)
    {
        if (!(node->value_.first < key))
        {
            candidate = node;
            node = node->left_;
        }
        else
            node = node->right_;
    }
    return candidate;
}
