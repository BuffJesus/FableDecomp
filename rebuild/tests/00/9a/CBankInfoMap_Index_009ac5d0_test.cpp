#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_retail_banks.h"

typedef CBankFileManager::CBankInfoMap Map;
static int insertions;
static int wideDefaults;
static int wideCopies;
static int wideDestructors;
static int charCopies;
static int charDestructors;
static Map::CEntryNode* inserted;
static Map::CNode* lastHint;

CWideString::CWideString() { memset(this, 0, sizeof(*this)); ++wideDefaults; }
CWideString::CWideString(const CWideString& other)
{
    memcpy(this, &other, sizeof(*this));
    ++wideCopies;
}
CWideString::~CWideString() { ++wideDestructors; }
CCharString::CCharString(const CCharString& other)
{
    memcpy(this, &other, sizeof(*this));
    ++charCopies;
}
CCharString::~CCharString() { ++charDestructors; }

Map::iterator Map::InsertUnique(iterator hint, const value_type& value)
{
    ++insertions;
    lastHint = hint.node_;
    inserted = static_cast<CEntryNode*>(calloc(1, sizeof(CEntryNode)));
    memcpy(&inserted->value_, &value, sizeof(value));
    return iterator(inserted);
}

static CBankFileManager::CBankInfo& index(Map& map, const CCharStringData* data)
{
    return map[*reinterpret_cast<const CCharString*>(&data)];
}

int main()
{
    CCharStringData alpha, bravo, zulu;
    alpha.Data.PData = "alpha";
    bravo.Data.PData = "bravo";
    zulu.Data.PData = "zulu";
    unsigned char mapBytes[sizeof(Map)] = {0};
    Map& map = *reinterpret_cast<Map*>(mapBytes);
    Map::CNode* head = static_cast<Map::CNode*>(calloc(1, sizeof(Map::CNode)));
    Map::CEntryNode* root = static_cast<Map::CEntryNode*>(calloc(1, sizeof(Map::CEntryNode)));
    map.head_ = head;

    CBankFileManager::CBankInfo* value = &index(map, &alpha);
    if (insertions != 1 || lastHint != head || value != &inserted->value_.second ||
        value->RegisteredBank != 0 || wideDefaults != 4 || wideCopies != 4 ||
        wideDestructors != 8 || charCopies != 1 || charDestructors != 1)
        return 1;
    free(inserted);
    inserted = 0;
    insertions = 0;

    head->parent_ = root;
    const CCharStringData* bravoData = &bravo;
    memcpy(reinterpret_cast<char*>(root) + 16, &bravoData, sizeof(bravoData));
    root->value_.second.RegisteredBank = reinterpret_cast<CBankFile*>(0x12345678);
    value = &index(map, &bravo);
    if (insertions != 0 || value != &root->value_.second ||
        value->RegisteredBank != reinterpret_cast<CBankFile*>(0x12345678) ||
        wideDefaults != 4 || wideCopies != 4) return 2;
    value = &index(map, &alpha);
    if (insertions != 1 || lastHint != root || value != &inserted->value_.second)
        return 3;
    free(inserted);
    inserted = 0;
    insertions = 0;
    value = &index(map, &zulu);
    if (insertions != 1 || lastHint != head || value != &inserted->value_.second)
        return 4;

    free(inserted);
    free(root);
    free(head);
    puts("BANK_INFO_INDEX_PASS");
    return 0;
}
