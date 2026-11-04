#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_retail_banks.h"

typedef CBankFileManager::CRetailBankList::CNode ListNode;
typedef CBankFileManager::CBankInfoMap::CNode MapNode;

static int deleted[4];
static int deletedCount;
static int erasedCount;
static MapNode* erasedRoot;

static void FABLE_FASTCALL deleteBank(void* value)
{
    deleted[deletedCount++] = *static_cast<int*>(value);
}

void CBankFileManager::CBankInfoMap::Erase(MapNode* root)
{
    ++erasedCount;
    erasedRoot = root;
}

static ListNode* makeNode(int* value, unsigned long owners)
{
    ListNode* node = static_cast<ListNode*>(malloc(sizeof(ListNode)));
    node->value_.Data = reinterpret_cast<NBankFileManager::CRetailBank*>(value);
    node->value_.Info = new CCPPointerInfo(&deleteBank, value);
    node->value_.Info->RefCount = owners;
    return node;
}

static void require(bool condition)
{
    if (!condition) { puts("BANK_CLOSE_FAIL"); exit(1); }
}

int main()
{
    unsigned char raw[sizeof(CBankFileManager)] = {0};
    CBankFileManager& manager = *reinterpret_cast<CBankFileManager*>(raw);
    unsigned char listHeadRaw[sizeof(ListNode)] = {0};
    ListNode& listHead = *reinterpret_cast<ListNode*>(listHeadRaw);
    listHead.next_ = listHead.prev_ = &listHead;
    manager.RetailBanks.node_ = &listHead;
    MapNode mapHead = {0};
    mapHead.left_ = mapHead.right_ = &mapHead;
    manager.Banks.head_ = &mapHead;

    manager.Close();
    require(deletedCount == 0 && erasedCount == 0 && listHead.next_ == &listHead);

    int values[3] = {10, 20, 30};
    ListNode* a = makeNode(&values[0], 1);
    ListNode* b = makeNode(&values[1], 2);
    ListNode* c = makeNode(&values[2], 1);
    CCPPointerInfo* shared = b->value_.Info;
    listHead.next_ = a; listHead.prev_ = c;
    a->prev_ = &listHead; a->next_ = b;
    b->prev_ = a; b->next_ = c;
    c->prev_ = b; c->next_ = &listHead;
    MapNode mapRoot = {0};
    mapHead.parent_ = &mapRoot;
    manager.Banks.size_ = 3;
    manager.Close();
    require(deletedCount == 2 && deleted[0] == 30 && deleted[1] == 10);
    require(shared->RefCount == 1 && listHead.next_ == &listHead && listHead.prev_ == &listHead);
    require(erasedCount == 1 && erasedRoot == &mapRoot && manager.Banks.size_ == 0);
    require(mapHead.left_ == &mapHead && mapHead.right_ == &mapHead && mapHead.parent_ == 0);
    delete shared;

    manager.Close();
    require(deletedCount == 2 && erasedCount == 1);
    puts("BANK_MANAGER_CLOSE_PASS");
    return 0;
}
