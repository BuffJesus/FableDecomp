#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_retail_banks.h"

typedef CBankFileManager::CBankInfoMap::CEntryNode Entry;
typedef CBankFileManager::CBankInfoMap::CNode MapNode;
typedef CBankFileManager::CRetailBankList::CNode ListNode;

static Entry* nodes[2];
static int keyOrder[2];
static int keyCount;
static int wideCount;

CCharString::~CCharString()
{
    keyOrder[keyCount++] = this == &nodes[1]->value_.first ? 1 : 0;
}
CWideString::~CWideString() { ++wideCount; }

static Entry* makeNode(int index)
{
    Entry* node = static_cast<Entry*>(malloc(sizeof(Entry)));
    memset(node, 0, sizeof(Entry));
    nodes[index] = node;
    return node;
}

int main()
{
    unsigned char raw[sizeof(CBankFileManager)] = {0};
    CBankFileManager& manager = *reinterpret_cast<CBankFileManager*>(raw);
    unsigned char listRaw[sizeof(ListNode)] = {0};
    ListNode& listHead = *reinterpret_cast<ListNode*>(listRaw);
    listHead.next_ = listHead.prev_ = &listHead;
    manager.RetailBanks.node_ = &listHead;
    MapNode mapHead = {0};
    manager.Banks.head_ = &mapHead;

    Entry* root = makeNode(0);
    Entry* right = makeNode(1);
    root->right_ = right;
    mapHead.parent_ = root;
    mapHead.left_ = root;
    mapHead.right_ = right;
    manager.Banks.size_ = 2;
    manager.Close();
    if (keyCount != 2 || keyOrder[0] != 1 || keyOrder[1] != 0 || wideCount != 8)
        return 1;
    if (manager.Banks.size_ != 0 || mapHead.parent_ != 0 ||
        mapHead.left_ != &mapHead || mapHead.right_ != &mapHead)
        return 2;
    puts("BANK_CLOSE_ERASE_COMPOSED_PASS");
    return 0;
}
