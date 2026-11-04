#include "fable_retail_banks.h"

CBankFileManager::CBankInfoMap::iterator&
CBankFileManager::CBankInfoMap::iterator::operator--()
{
    CNode* node = node_;
    if (!node->color_ && node->parent_->parent_ == node)
    {
        node_ = node->right_;
        return *this;
    }
    CNode* previous = node->left_;
    if (previous != 0)
    {
        while (previous->right_ != 0)
            previous = previous->right_;
        node_ = previous;
    }
    else
    {
        CNode* parent = node->parent_;
        while (node == parent->left_)
        {
            node = parent;
            parent = parent->parent_;
        }
        node_ = parent;
    }
    return *this;
}
