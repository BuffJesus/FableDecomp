#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_retail_banks.h"

static void setKey(void* node, const CCharStringData* data)
{
    memcpy(static_cast<char*>(node) + 16, &data, sizeof(data));
}

template<class Map>
static typename Map::CNode* lookup(Map& map, const CCharStringData* data)
{
    return map.LowerBound(*reinterpret_cast<const CCharString*>(&data));
}

template<class Map>
static bool checkMap()
{
    typedef typename Map::CNode Node;
    CCharStringData able;
    CCharStringData alpha;
    CCharStringData bravo;
    CCharStringData between;
    CCharStringData zulu;
    able.Data.PData = "able";
    alpha.Data.PData = "alpha";
    bravo.Data.PData = "bravo";
    between.Data.PData = "azzz";
    zulu.Data.PData = "zulu";
    unsigned char mapBytes[sizeof(Map)] = {0};
    Map& map = *reinterpret_cast<Map*>(mapBytes);
    Node* head = static_cast<Node*>(calloc(1, 64));
    Node* root = static_cast<Node*>(calloc(1, 64));
    Node* left = static_cast<Node*>(calloc(1, 64));
    Node* right = static_cast<Node*>(calloc(1, 64));
    map.head_ = head;
    bool ok = lookup(map, &alpha) == head;
    head->parent_ = root;
    root->left_ = left;
    root->right_ = right;
    setKey(root, &alpha);
    setKey(left, &able);
    setKey(right, &bravo);
    ok = ok && lookup(map, &able) == left;
    ok = ok && lookup(map, &alpha) == root;
    ok = ok && lookup(map, &between) == right;
    ok = ok && lookup(map, &zulu) == head;
    free(right);
    free(left);
    free(root);
    free(head);
    return ok;
}

int main()
{
    if (!checkMap<NBankFileManager::CContainedBankMap>()) return 1;
    if (!checkMap<CBankFileManager::CBankInfoMap>()) return 2;
    if (!checkMap<CBasicRetailBankAliasMap>()) return 3;
    puts("BANK_MAPS_LOWER_BOUND_PASS");
    return 0;
}
