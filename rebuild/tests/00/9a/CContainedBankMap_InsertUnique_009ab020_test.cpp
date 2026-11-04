#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_retail_banks.h"

typedef NBankFileManager::CContainedBankMap Map;

CCharString::CCharString(const CCharString& other)
{
    memcpy(this, &other, sizeof(*this));
}

static const Map::value_type& valueFor(fable_u32* raw,
                                      const CCharStringData* key,
                                      fable_u32 marker)
{
    memcpy(raw, &key, sizeof(key));
    for (int i = 1; i < 6; ++i) raw[i] = marker + i;
    return *reinterpret_cast<const Map::value_type*>(raw);
}

int main()
{
    CCharStringData alpha, azure, bravo, charlie;
    alpha.Data.PData = "alpha";
    azure.Data.PData = "azure";
    bravo.Data.PData = "bravo";
    charlie.Data.PData = "charlie";
    fable_u32 a[6], az[6], b[6], c[6];
    unsigned char mapBytes[sizeof(Map)] = {0};
    Map& map = *reinterpret_cast<Map*>(mapBytes);
    Map::CNode* head = static_cast<Map::CNode*>(calloc(1, sizeof(Map::CNode)));
    head->left_ = head;
    head->right_ = head;
    map.head_ = head;

    std::pair<Map::iterator, bool> root = map.InsertUnique(valueFor(b, &bravo, 10));
    if (!root.second || root.first.node_ == head || map.size_ != 1 ||
        head->parent_ != root.first.node_) return 1;
    std::pair<Map::iterator, bool> left = map.InsertUnique(valueFor(a, &alpha, 20));
    if (!left.second || head->left_ != left.first.node_ || map.size_ != 2) return 2;
    std::pair<Map::iterator, bool> right = map.InsertUnique(valueFor(c, &charlie, 30));
    if (!right.second || head->right_ != right.first.node_ || map.size_ != 3) return 3;
    std::pair<Map::iterator, bool> same = map.InsertUnique(valueFor(b, &bravo, 90));
    if (same.second || same.first.node_ != root.first.node_ || map.size_ != 3 ||
        same.first.node_->value_.second.dataVersion_ != 11) return 4;
    std::pair<Map::iterator, bool> middle = map.InsertUnique(valueFor(az, &azure, 40));
    const CCharStringData* stored = 0;
    memcpy(&stored, &middle.first.node_->value_.first, sizeof(stored));
    if (!middle.second || middle.first.node_ == head || map.size_ != 4 ||
        strcmp(stored->Data.PData, "azure")) return 5;

    puts("CONTAINED_BANK_INSERT_UNIQUE_PASS");
    return 0;
}
