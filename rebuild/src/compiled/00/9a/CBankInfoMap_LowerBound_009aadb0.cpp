#include "fable_retail_banks.h"

CBankFileManager::CBankInfoMap::CNode*
CBankFileManager::CBankInfoMap::LowerBound(const CCharString& key) const
{
    CNode* candidate = head_;
    CNode* node = head_->parent_;
    while (node != 0)
    {
        const CEntryNode* entry = static_cast<const CEntryNode*>(node);
        if (!(entry->value_.first < key))
        {
            candidate = node;
            node = node->left_;
        }
        else
            node = node->right_;
    }
    return candidate;
}
