#include "fable_retail_banks.h"

void CBankFileManager::CBankInfoMap::Erase(CNode* node)
{
    while (node != 0)
    {
        Erase(node->right_);
        CNode* left = node->left_;
        CEntryNode* entry = static_cast<CEntryNode*>(node);
        entry->~CEntryNode();
        free(node);
        node = left;
    }
}
