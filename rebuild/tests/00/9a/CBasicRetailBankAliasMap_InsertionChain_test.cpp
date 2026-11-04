#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_retail_banks.h"

typedef CBasicRetailBankAliasMap Map;

CCharString::CCharString() { memset(this, 0, sizeof(*this)); }
CCharString::CCharString(const CCharString& other) { memcpy(this, &other, sizeof(*this)); }
CCharString::~CCharString() {}

static CCharString key(const CCharStringData* data)
{
    CCharString result;
    memcpy(&result, &data, sizeof(data));
    return result;
}

static const Map::value_type& valueFor(fable_u32* raw, const CCharStringData* data)
{
    memcpy(raw, &data, sizeof(data));
    raw[1] = 0;
    return *reinterpret_cast<const Map::value_type*>(raw);
}

int main()
{
    CCharStringData alpha, azure, bravo, charlie, zulu;
    alpha.Data.PData = "alpha";
    azure.Data.PData = "azure";
    bravo.Data.PData = "bravo";
    charlie.Data.PData = "charlie";
    zulu.Data.PData = "zulu";
    unsigned char mapBytes[sizeof(Map)] = {0};
    Map& map = *reinterpret_cast<Map*>(mapBytes);
    Map::CNode* head = static_cast<Map::CNode*>(calloc(1, sizeof(Map::CNode)));
    head->left_ = head;
    head->right_ = head;
    map.head_ = head;

    CCharString* first = &map[key(&bravo)];
    if (!first || map.size_ != 1 || head->parent_ == 0) return 1;
    if (&map[key(&bravo)] != first || map.size_ != 1) return 2;
    if (&map[key(&alpha)] == first || map.size_ != 2) return 3;
    if (&map[key(&charlie)] == first || map.size_ != 3) return 4;
    if (&map[key(&azure)] == first || map.size_ != 4) return 5;

    Map::CNode* left = map.LowerBound(key(&alpha));
    Map::CNode* root = map.LowerBound(key(&bravo));
    if (left == head || root == head) return 6;
    fable_u32 raw[2];
    Map::iterator far = map.InsertUnique(Map::iterator(left), valueFor(raw, &zulu));
    if (far.node_ == head || head->right_ != far.node_ || map.size_ != 5) return 7;
    std::pair<Map::iterator, bool> duplicate = map.InsertUnique(valueFor(raw, &bravo));
    if (duplicate.second || duplicate.first.node_ != root || map.size_ != 5) return 8;

    puts("RETAIL_BANK_ALIAS_INSERTION_CHAIN_PASS");
    return 0;
}
