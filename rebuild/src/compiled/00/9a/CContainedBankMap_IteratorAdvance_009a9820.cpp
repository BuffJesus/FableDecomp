#include "fable_retail_banks.h"

NBankFileManager::CContainedBankMap::iterator&
NBankFileManager::CContainedBankMap::iterator::operator++()
{
    CNode* node = node_;
    CNode* next = node->right_;
    if (next != 0)
    {
        CNode* left = next->left_;
        while (left != 0)
        {
            next = left;
            left = next->left_;
        }
        node_ = next;
        return *this;
    }
    else
    {
        CNode* parent = node->parent_;
        while (node == parent->right_)
        {
            node = parent;
            parent = parent->parent_;
        }
        node_ = node->right_ != parent ? parent : node;
    }
    return *this;
}
