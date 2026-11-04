#include "fable_retail_banks.h"

void NBankFileManager::CContainedBankMap::Erase(CNode* node)
{
    while (node != 0)
    {
        Erase(node->right_);
        CNode* left = node->left_;
        node->value_.~pair();
        free(node);
        node = left;
    }
}
