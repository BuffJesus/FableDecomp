#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_retail_banks.h"

typedef NBankFileManager::CContainedBankMap Map;
typedef Map::CNode Node;

CCharString::CCharString(const CCharString& other)
{
    memcpy(this, &other, sizeof(*this));
}

static const Map::value_type& valueFor(fable_u32* raw,
                                      const CCharStringData* key)
{
    memcpy(raw, &key, sizeof(key));
    for (int i = 1; i < 6; ++i) raw[i] = static_cast<fable_u32>(i);
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
    fable_u32 a[6], az[6], b[6], c[6], z[6];
    unsigned char mapBytes[sizeof(Map)] = {0};
    Map& map = *reinterpret_cast<Map*>(mapBytes);
    Node* head = static_cast<Node*>(calloc(1, sizeof(Node)));
    head->left_ = head;
    head->right_ = head;
    map.head_ = head;

    Node* root = map.InsertUnique(Map::iterator(head), valueFor(b, &bravo)).node_;
    if (root == head || map.size_ != 1) return 1;
    if (map.InsertUnique(Map::iterator(root), valueFor(b, &bravo)).node_ != root ||
        map.size_ != 1) return 2;
    Node* left = map.InsertUnique(Map::iterator(root), valueFor(a, &alpha)).node_;
    if (left == head || map.size_ != 2 || head->left_ != left)
        return 3;
    Node* right = map.InsertUnique(Map::iterator(head), valueFor(c, &charlie)).node_;
    if (right == head || map.size_ != 3 || head->right_ != right)
        return 4;
    Node* middle = map.InsertUnique(Map::iterator(root), valueFor(az, &azure)).node_;
    if (middle == head || map.size_ != 4) return 5;
    Node* far = map.InsertUnique(Map::iterator(left), valueFor(z, &zulu)).node_;
    if (far == head || head->right_ != far || map.size_ != 5) return 6;

    puts("CONTAINED_BANK_INSERT_HINT_PASS");
    return 0;
}
