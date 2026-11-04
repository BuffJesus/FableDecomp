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

static const Map::value_type& makeValue(fable_u32* raw,
                                        const CCharStringData* key,
                                        fable_u32 marker)
{
    memcpy(raw, &key, sizeof(key));
    for (int i = 1; i < 6; ++i) raw[i] = marker + i;
    return *reinterpret_cast<const Map::value_type*>(raw);
}

int main()
{
    CCharStringData alpha;
    CCharStringData bravo;
    CCharStringData charlie;
    alpha.Data.PData = "alpha";
    bravo.Data.PData = "bravo";
    charlie.Data.PData = "charlie";
    fable_u32 rootRaw[6], leftRaw[6], rightRaw[6];
    unsigned char mapBytes[sizeof(Map)] = {0};
    Map& map = *reinterpret_cast<Map*>(mapBytes);
    Node* head = static_cast<Node*>(calloc(1, sizeof(Node)));
    head->left_ = head;
    head->right_ = head;
    map.head_ = head;

    Node* root = map.InsertNode(0, head, makeValue(rootRaw, &bravo, 10), 0).node_;
    if (!root || head->parent_ != root || head->left_ != root ||
        head->right_ != root || root->parent_ != head ||
        root->left_ || root->right_ || !root->color_ || map.size_ != 1)
        return 1;
    Node* left = map.InsertNode(0, root, makeValue(leftRaw, &alpha, 20), 0).node_;
    if (!left || root->left_ != left || head->left_ != left ||
        left->parent_ != root || map.size_ != 2 ||
        left->value_.second.dataVersion_ != 21)
        return 2;
    Node* right = map.InsertNode(0, root, makeValue(rightRaw, &charlie, 30), 1).node_;
    if (!right || root->right_ != right || head->right_ != right ||
        right->parent_ != root || map.size_ != 3 ||
        right->value_.second.alignment_ != 35)
        return 3;
    if (head->parent_ != root || !root->color_) return 4;

    free(right);
    free(left);
    free(root);
    free(head);
    puts("CONTAINED_BANK_INSERT_NODE_PASS");
    return 0;
}
