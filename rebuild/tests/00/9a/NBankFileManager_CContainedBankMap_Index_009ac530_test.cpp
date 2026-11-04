#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fable_retail_banks.h"

// InsertUnique below records the request without populating the owning tree.
// Its destructor must therefore only release the empty sentinel.
void NBankFileManager::CContainedBankMap::Erase(CNode*) { abort(); }

namespace
{
    typedef NBankFileManager::CContainedBankMap Map;
    typedef Map::CNode Node;

    const CCharString* g_expectedKey;
    Node* g_lowerBound;
    Node* g_inserted;
    const CCharStringData* g_lessLeft;
    const CCharStringData* g_lessRight;
    const void* g_copied;
    const void* g_destroyed;
    unsigned int g_lessCalls;
    unsigned int g_copyCalls;
    unsigned int g_insertCalls;
    unsigned int g_destroyCalls;
    bool g_lessResult;
    bool g_argumentsValid;

    CCharStringData g_keyData;
    CCharStringData g_nodeData;

    void SetStorage(const CCharString& string, CCharStringData* data)
    {
        *reinterpret_cast<CCharStringData**>(
            const_cast<CCharString*>(&string)) = data;
    }

    CCharStringData* GetStorage(const CCharString& string)
    {
        return *reinterpret_cast<CCharStringData* const*>(&string);
    }

    // Runs map[key] with the key/node string data given; returns true when
    // the reference came back from the expected node and the insert path
    // ran exactly when expected.
    bool Run(
        CCharStringData* keyData,
        CCharStringData* nodeData,
        bool atEnd,
        bool lessResult,
        bool expectLess,
        bool expectInsert)
    {
        g_lessCalls = 0;
        g_copyCalls = 0;
        g_insertCalls = 0;
        g_destroyCalls = 0;
        g_copied = 0;
        g_destroyed = 0;
        g_lessResult = lessResult;
        // The data comparison is CBasicString<char>::operator< (a signed
        // strcmp of the texts): give the two data texts that order.
        if (keyData != 0 && nodeData != 0 && keyData != nodeData)
        {
            keyData->Data.PData = const_cast<char*>(lessResult ? "a" : "b");
            nodeData->Data.PData = const_cast<char*>(lessResult ? "b" : "a");
        }
        g_argumentsValid = true;

        Map map;
        Node* head = map.head_;
        static Node node;
        static Node inserted;
        memset(&node, 0, sizeof(node));
        memset(&inserted, 0, sizeof(inserted));
        SetStorage(node.value_.first, nodeData);
        const_cast<NBankFileManager::CContainedBank&>(
            node.value_.second).entryCount_ = 17;

        CCharString key;
        SetStorage(key, keyData);
        g_expectedKey = &key;
        g_lowerBound = atEnd ? head : &node;
        g_inserted = &inserted;
        g_lessLeft = keyData;
        g_lessRight = nodeData;

        NBankFileManager::CContainedBank& value = map[key];
        SetStorage(key, 0);

        const bool paths =
            g_copyCalls == (expectInsert ? 1u : 0u) &&
            g_insertCalls == (expectInsert ? 1u : 0u) &&
            g_destroyCalls == (expectInsert ? 1u : 0u) &&
            g_copied == g_destroyed &&
            g_argumentsValid;
        if (expectInsert)
            return paths && &value == &inserted.value_.second;
        return paths && &value == &node.value_.second &&
            value.entryCount_ == 17;
    }
}

CCharString::CCharString()
{
    storage_ = 0;
}

CCharString::CCharString(const CCharString& other)
{
    ++g_copyCalls;
    g_argumentsValid = g_argumentsValid && &other == g_expectedKey;
    g_copied = this;
    storage_ = other.storage_;
}

CCharString::~CCharString()
{
    if (g_copied != 0 && this == g_copied)
    {
        ++g_destroyCalls;
        g_destroyed = this;
    }
}

NBankFileManager::CContainedBankMap::CNode*
NBankFileManager::CContainedBankMap::LowerBound(const CCharString& key) const
{
    g_argumentsValid = g_argumentsValid && &key == g_expectedKey;
    return g_lowerBound;
}

NBankFileManager::CContainedBankMap::iterator
NBankFileManager::CContainedBankMap::InsertUnique(
    iterator hint,
    const value_type& value)
{
    ++g_insertCalls;
    const NBankFileManager::CContainedBank zero;
    g_argumentsValid =
        g_argumentsValid &&
        hint.node_ == g_lowerBound &&
        &value.first == g_copied &&
        GetStorage(value.first) == GetStorage(*g_expectedKey) &&
        memcmp(&value.second, &zero, sizeof(zero)) == 0;
    return iterator(g_inserted);
}

int main()
{
    const bool passed =
        // lower_bound hit the end: insert.
        Run(&g_keyData, &g_nodeData, true, false, false, true) &&
        // Same shared string data: found without comparing.
        Run(&g_keyData, &g_keyData, false, true, false, false) &&
        // Null key sorts first: insert.
        Run(0, &g_nodeData, false, false, false, true) &&
        // Null node key: key is not less, found.
        Run(&g_keyData, 0, false, true, false, false) &&
        // Distinct data: the data comparison decides.
        Run(&g_keyData, &g_nodeData, false, true, true, true) &&
        Run(&g_keyData, &g_nodeData, false, false, true, false);
    if (!passed)
    {
        printf("FABLETLC_CONTAINED_BANK_MAP_INDEX FAIL\n");
        return 1;
    }

    printf("FABLETLC_CONTAINED_BANK_MAP_INDEX PASS\n");
    return 0;
}
