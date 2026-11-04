#include "fable_retail_banks.h"

template<class Node>
__forceinline const CCharString& BankKey(const Node* node)
{
    return node->value_.first;
}

template<>
__forceinline const CCharString& BankKey(const CBankFileManager::CBankInfoMap::CNode* node)
{
    return static_cast<const CBankFileManager::CBankInfoMap::CEntryNode*>(node)->value_.first;
}

template<class Node>
__forceinline Node* LowerBoundBank(Node* head, const CCharString& key)
{
    Node* candidate = head;
    Node* node = head->parent_;
    while (node)
    {
        if (!(BankKey(node) < key))
        {
            candidate = node;
            node = node->left_;
        }
        else
            node = node->right_;
    }
    return candidate;
}

template<class Node>
__forceinline Node* FindBank(Node* head, const CCharString& key)
{
    Node* candidate = LowerBoundBank(head, key);
    if (candidate == head || key < BankKey(candidate))
        candidate = head;
    return candidate;
}

NBankFileManager::CContainedBankMap::CNode*
NBankFileManager::CContainedBankMap::Find(const CCharString& key) const
{
    return FindBank(head_, key);
}

CBankFileManager::CBankInfoMap::CNode*
CBankFileManager::CBankInfoMap::Find(const CCharString& key) const
{
    return FindBank(head_, key);
}

CBasicRetailBankAliasMap::CNode*
CBasicRetailBankAliasMap::Find(const CCharString& key) const
{
    return FindBank(head_, key);
}
