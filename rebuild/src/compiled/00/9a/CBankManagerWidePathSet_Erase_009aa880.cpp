#include "fable_retail_banks.h"

void CBankManagerWidePathSet::Erase(CNode* node)
{
    while (node != 0)
    {
        Erase(node->right_);
        CNode* left = node->left_;
        node->value_.~CWideString();
        free(node);
        node = left;
    }
}
