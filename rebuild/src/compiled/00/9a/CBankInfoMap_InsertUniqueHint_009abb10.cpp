#include "fable_retail_banks.h"
#include "fable_stlport_tree.h"

CBankFileManager::CBankInfoMap::iterator
CBankFileManager::CBankInfoMap::InsertUnique(
    iterator position, const value_type& value)
{
    const _STL::less<CCharString>& less =
        *reinterpret_cast<const _STL::less<CCharString>*>(&compare_);
    if (position.node_ == head_->left_)
    {
        if (size_ <= 0)
            return InsertUnique(value).first;

        if (less(value.first, static_cast<CEntryNode*>(position.node_)->value_.first))
            return InsertNode(position.node_, position.node_, value, 0);
        else
        {
            bool compPosValue = less(static_cast<CEntryNode*>(position.node_)->value_.first, value.first);
            if (compPosValue == false)
                return position;

            iterator after = position;
            ++after;
            if (after.node_ == head_)
                return InsertNode(0, position.node_, value,
                                  reinterpret_cast<fable_u32>(position.node_));
            if (less(value.first, static_cast<CEntryNode*>(after.node_)->value_.first))
            {
                if (position.node_->right_ == 0)
                    return InsertNode(0, position.node_, value,
                                      reinterpret_cast<fable_u32>(position.node_));
                else
                    return InsertNode(after.node_, after.node_, value, 0);
            }
            else
                return InsertUnique(value).first;
        }
    }
    else if (position.node_ == head_)
    {
        if (less(static_cast<CEntryNode*>(head_->right_)->value_.first, value.first))
            return InsertNode(0, head_->right_, value,
                              reinterpret_cast<fable_u32>(position.node_));
        else
            return InsertUnique(value).first;
    }
    else
    {
        iterator before = position;
        --before;
        bool compValuePos = less(value.first, static_cast<CEntryNode*>(position.node_)->value_.first);
        if (compValuePos && less(static_cast<CEntryNode*>(before.node_)->value_.first, value.first))
        {
            if (before.node_->right_ == 0)
                return InsertNode(0, before.node_, value,
                                  reinterpret_cast<fable_u32>(before.node_));
            else
                return InsertNode(position.node_, position.node_, value, 0);
        }
        else
        {
            iterator after = position;
            ++after;
            bool compPosValue = !compValuePos;
            if (!compValuePos)
                compPosValue = less(static_cast<CEntryNode*>(position.node_)->value_.first, value.first);
            if ((!compValuePos) && compPosValue &&
                (after.node_ == head_ || less(value.first, static_cast<CEntryNode*>(after.node_)->value_.first)))
            {
                if (position.node_->right_ == 0)
                    return InsertNode(0, position.node_, value,
                                      reinterpret_cast<fable_u32>(position.node_));
                else
                    return InsertNode(after.node_, after.node_, value, 0);
            }
            else
            {
                if (compValuePos == compPosValue)
                    return position;
                else
                    return InsertUnique(value).first;
            }
        }
    }
}
