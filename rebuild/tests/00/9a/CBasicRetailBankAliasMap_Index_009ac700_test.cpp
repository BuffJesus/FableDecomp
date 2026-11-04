#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_retail_banks.h"

typedef CBasicRetailBankAliasMap::CNode Node;
static int inserts;
static Node* inserted;
static Node* lastHint;

CCharString::CCharString() { memset(this, 0, sizeof(*this)); }
CCharString::CCharString(const CCharString& other) { memcpy(this, &other, sizeof(*this)); }
CCharString::~CCharString() {}

CBasicRetailBankAliasMap::iterator
CBasicRetailBankAliasMap::InsertUnique(iterator hint, const value_type& value)
{
    ++inserts;
    lastHint = hint.node_;
    inserted = static_cast<Node*>(calloc(1, sizeof(Node)));
    memcpy(reinterpret_cast<char*>(inserted) + 16, &value.first, sizeof(CCharString));
    return iterator(inserted);
}

static CCharString key(const CCharStringData* data)
{
    CCharString result;
    memcpy(&result, &data, sizeof(data));
    return result;
}

int main()
{
    CCharStringData alpha;
    CCharStringData bravo;
    CCharStringData zulu;
    alpha.Data.PData = "alpha";
    bravo.Data.PData = "bravo";
    zulu.Data.PData = "zulu";
    CBasicRetailBankAliasMap map = {};
    Node* head = static_cast<Node*>(calloc(1, sizeof(Node)));
    Node* root = static_cast<Node*>(calloc(1, sizeof(Node)));
    map.head_ = head;

    CCharString* value = &map[key(&alpha)];
    if (inserts != 1 || lastHint != head || value != &inserted->value_.second)
        return 1;
    free(inserted);
    inserted = 0;
    inserts = 0;

    head->parent_ = root;
    const CCharStringData* bravoData = &bravo;
    memcpy(reinterpret_cast<char*>(root) + 16, &bravoData, sizeof(bravoData));
    value = &map[key(&bravo)];
    if (inserts != 0 || value != &root->value_.second) return 2;
    value = &map[key(&alpha)];
    if (inserts != 1 || lastHint != root || value != &inserted->value_.second)
        return 3;
    free(inserted);
    inserted = 0;
    inserts = 0;
    value = &map[key(&zulu)];
    if (inserts != 1 || lastHint != head || value != &inserted->value_.second)
        return 4;

    free(inserted);
    free(root);
    free(head);
    puts("RETAIL_BANK_ALIAS_INDEX_PASS");
    return 0;
}
