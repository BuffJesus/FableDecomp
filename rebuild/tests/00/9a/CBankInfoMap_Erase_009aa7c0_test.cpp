#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_retail_banks.h"

typedef CBankFileManager::CBankInfoMap::CNode Node;
typedef CBankFileManager::CBankInfoMap::CEntryNode Entry;

static Entry* nodes[3];
static int keyOrder[3];
static int keyCount;
static const CWideString* wideOrder[12];
static const CWideString* expectedWide[12];
static int wideCount;

static int nodeId(const CCharString* key)
{
    for (int i = 0; i < 3; ++i)
        if (key == &nodes[i]->value_.first)
            return i;
    return -1;
}

CCharString::~CCharString()
{
    keyOrder[keyCount++] = nodeId(this);
}
CWideString::~CWideString()
{
    wideOrder[wideCount++] = this;
}

static Entry* makeNode(int id)
{
    Entry* node = static_cast<Entry*>(malloc(sizeof(Entry)));
    memset(node, 0, sizeof(Entry));
    nodes[id] = node;
    return node;
}

static void saveWideFields(Entry* node, int index)
{
    const CBankFileManager::CBankInfo& info = node->value_.second;
    expectedWide[index] = &info.RetailHeaderFilePath;
    expectedWide[index + 1] = &info.DevHeaderFilePath;
    expectedWide[index + 2] = &info.RetailBankFilePath;
    expectedWide[index + 3] = &info.DevBankFilePath;
}

int main()
{
    unsigned char mapBytes[sizeof(CBankFileManager::CBankInfoMap)] = {0};
    CBankFileManager::CBankInfoMap& map =
        *reinterpret_cast<CBankFileManager::CBankInfoMap*>(mapBytes);
    map.Erase(0);
    if (keyCount || wideCount) return 1;

    Entry* root = makeNode(0);
    Entry* left = makeNode(1);
    Entry* right = makeNode(2);
    root->left_ = left;
    root->right_ = right;
    saveWideFields(right, 0);
    saveWideFields(root, 4);
    saveWideFields(left, 8);
    map.Erase(root);
    if (keyCount != 3 || wideCount != 12) return 2;
    if (keyOrder[0] != 2 || keyOrder[1] != 0 || keyOrder[2] != 1) return 3;
    for (int i = 0; i < 12; ++i)
        if (wideOrder[i] != expectedWide[i]) return 4;
    puts("BANK_INFO_ERASE_PASS");
    return 0;
}
