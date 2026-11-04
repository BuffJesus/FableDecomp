#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_retail_banks.h"

typedef NBankFileManager::CContainedBankMap::CNode Node;
static Node* nodes[3];
static int order[3];
static int calls;

CCharString::~CCharString()
{
    for (int i = 0; i < 3; ++i)
        if (this == &nodes[i]->value_.first)
            order[calls++] = i;
}

static Node* makeNode(int index)
{
    Node* result = static_cast<Node*>(malloc(sizeof(Node)));
    memset(result, 0, sizeof(Node));
    nodes[index] = result;
    return result;
}

int main()
{
    unsigned char raw[sizeof(NBankFileManager::CContainedBankMap)] = {0};
    NBankFileManager::CContainedBankMap& map =
        *reinterpret_cast<NBankFileManager::CContainedBankMap*>(raw);
    map.Erase(0);
    if (calls) return 1;
    Node* root = makeNode(0);
    root->left_ = makeNode(1);
    root->right_ = makeNode(2);
    map.Erase(root);
    if (calls != 3 || order[0] != 2 || order[1] != 0 || order[2] != 1)
        return 2;
    puts("CONTAINED_BANK_ERASE_PASS");
    return 0;
}
