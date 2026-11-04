#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_retail_banks.h"

static const void* destroyed[10];
static int count;
static int released;
static void FABLE_FASTCALL release(void*) { ++released; }
CCharString::~CCharString() { destroyed[count++] = this; }
CWideString::~CWideString() { destroyed[count++] = this; }

int main()
{
    typedef CBasicRetailBankAliasMap::CNode AliasNode;
    typedef CBankManagerWidePathSet::CNode WideNode;
    typedef CBankFileManager::CRetailBankList::CNode ListNode;
    CBasicRetailBankAliasMap aliasMap = {0};
    CBankManagerWidePathSet wideMap = {0};
    CBankFileManager::CRetailBankList list = {0};
    aliasMap.Erase(0);
    wideMap.Erase(0);
    if (count) return 1;

    AliasNode* a = static_cast<AliasNode*>(calloc(1, sizeof(AliasNode)));
    AliasNode* b = static_cast<AliasNode*>(calloc(1, sizeof(AliasNode)));
    a->right_ = b;
    const void* expectAlias[] = { &b->value_.second, &b->value_.first,
                                  &a->value_.second, &a->value_.first };
    aliasMap.Erase(a);
    if (count != 4) return 2;
    for (int i = 0; i < 4; ++i) if (destroyed[i] != expectAlias[i]) return 3;

    WideNode* w = static_cast<WideNode*>(calloc(1, sizeof(WideNode)));
    WideNode* x = static_cast<WideNode*>(calloc(1, sizeof(WideNode)));
    w->left_ = x;
    const void* expectWide[] = { &w->value_, &x->value_ };
    wideMap.Erase(w);
    if (count != 6) return 4;
    for (int i = 0; i < 2; ++i) if (destroyed[4 + i] != expectWide[i]) return 5;

    ListNode* head = static_cast<ListNode*>(calloc(1, sizeof(ListNode)));
    ListNode* first = static_cast<ListNode*>(calloc(1, sizeof(ListNode)));
    ListNode* second = static_cast<ListNode*>(calloc(1, sizeof(ListNode)));
    CCPPointerInfo* info = static_cast<CCPPointerInfo*>(malloc(sizeof(CCPPointerInfo)));
    info->RefCount = 2;
    info->DeleteFunc = release;
    info->Data = 0;
    first->value_.Info = info;
    second->value_.Info = info;
    head->next_ = first;
    head->prev_ = second;
    first->next_ = second;
    first->prev_ = head;
    second->next_ = head;
    second->prev_ = first;
    list.node_ = head;
    list.clear();
    if (released != 1 || head->next_ != head || head->prev_ != head) return 6;
    list.clear();
    if (released != 1) return 7;
    free(head);
    puts("BANK_MANAGER_CONTAINERS_PASS");
    return 0;
}
