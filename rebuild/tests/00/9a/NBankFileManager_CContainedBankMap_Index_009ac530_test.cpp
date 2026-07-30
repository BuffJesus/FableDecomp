#include <stdio.h>
#include <string.h>

#include "fable_retail_banks.h"

namespace
{
    struct TestNode
    {
        fable_u8 linksAndFlags_[0x10];
        void* keyData_;
        NBankFileManager::CContainedBank value_;
    };

    CCharString* g_expectedKey;
    TestNode* g_lowerBound;
    TestNode* g_inserted;
    unsigned int g_copyCalls;
    unsigned int g_insertCalls;
    unsigned int g_destroyCalls;
    bool g_lessResult;
    bool g_argumentsValid;

    void Reset()
    {
        g_copyCalls = 0;
        g_insertCalls = 0;
        g_destroyCalls = 0;
        g_lessResult = false;
        g_argumentsValid = true;
    }

    bool CheckExistingEqualPointer()
    {
        Reset();
        NBankFileManager::CContainedBankMap map = {};
        TestNode head = {};
        TestNode node = {};
        fable_u8 keyBytes[sizeof(CCharString)] = {};
        CCharString* key = reinterpret_cast<CCharString*>(keyBytes);
        void* data = reinterpret_cast<void*>(0x11110000);
        *reinterpret_cast<void**>(keyBytes) = data;
        node.keyData_ = data;
        node.value_.entryCount_ = 17;
        map.head_ = &head;
        g_expectedKey = key;
        g_lowerBound = &node;

        NBankFileManager::CContainedBank& value = map[*key];
        return
            &value == &node.value_ &&
            value.entryCount_ == 17 &&
            g_copyCalls == 0 &&
            g_insertCalls == 0 &&
            g_destroyCalls == 0 &&
            g_argumentsValid;
    }

    bool CheckExistingOrdered()
    {
        Reset();
        NBankFileManager::CContainedBankMap map = {};
        TestNode head = {};
        TestNode node = {};
        fable_u8 keyBytes[sizeof(CCharString)] = {};
        CCharString* key = reinterpret_cast<CCharString*>(keyBytes);
        *reinterpret_cast<void**>(keyBytes) =
            reinterpret_cast<void*>(0x22220000);
        node.keyData_ = reinterpret_cast<void*>(0x33330000);
        node.value_.alignment_ = 2048;
        map.head_ = &head;
        g_expectedKey = key;
        g_lowerBound = &node;
        g_lessResult = false;

        NBankFileManager::CContainedBank& value = map[*key];
        return
            &value == &node.value_ &&
            value.alignment_ == 2048 &&
            g_copyCalls == 0 &&
            g_insertCalls == 0 &&
            g_destroyCalls == 0 &&
            g_argumentsValid;
    }

    bool CheckInsert(bool atEnd)
    {
        Reset();
        NBankFileManager::CContainedBankMap map = {};
        TestNode head = {};
        TestNode candidate = {};
        TestNode inserted = {};
        fable_u8 keyBytes[sizeof(CCharString)] = {};
        CCharString* key = reinterpret_cast<CCharString*>(keyBytes);
        *reinterpret_cast<void**>(keyBytes) =
            reinterpret_cast<void*>(0x44440000);
        candidate.keyData_ = reinterpret_cast<void*>(0x55550000);
        map.head_ = &head;
        g_expectedKey = key;
        g_lowerBound = atEnd ? &head : &candidate;
        g_inserted = &inserted;
        g_lessResult = true;

        NBankFileManager::CContainedBank& value = map[*key];
        const NBankFileManager::CContainedBank zero = {};
        return
            &value == &inserted.value_ &&
            memcmp(&value, &zero, sizeof(value)) == 0 &&
            g_copyCalls == 1 &&
            g_insertCalls == 1 &&
            g_destroyCalls == 1 &&
            g_argumentsValid;
    }
}

extern "C" void* FABLE_FASTCALL
FableContainedBankMapLowerBound_009AC530(
    NBankFileManager::CContainedBankMap*,
    void*,
    const CCharString* key)
{
    g_argumentsValid = g_argumentsValid && key == g_expectedKey;
    return g_lowerBound;
}

extern "C" bool FABLE_FASTCALL
FableContainedBankMapStringLess_009AC530(
    const void* leftData,
    void*,
    const void* rightData)
{
    g_argumentsValid =
        g_argumentsValid &&
        leftData == *reinterpret_cast<void**>(g_expectedKey) &&
        rightData == g_lowerBound->keyData_;
    return g_lessResult;
}

extern "C" void FABLE_FASTCALL
FableContainedBankMapStringCopy_009AC530(
    void* destination,
    void*,
    const CCharString* source)
{
    ++g_copyCalls;
    g_argumentsValid =
        g_argumentsValid &&
        destination != 0 &&
        source == g_expectedKey;
    *reinterpret_cast<void**>(destination) =
        *reinterpret_cast<void* const*>(source);
}

extern "C" void* FABLE_FASTCALL
FableContainedBankMapInsert_009AC530(
    NBankFileManager::CContainedBankMap*,
    void*,
    void** result,
    void* hint,
    const void* value)
{
    ++g_insertCalls;
    g_argumentsValid =
        g_argumentsValid &&
        result != 0 &&
        hint == g_lowerBound &&
        value != 0;
    *result = g_inserted;
    return result;
}

extern "C" void FABLE_FASTCALL
FableContainedBankMapStringDestroy_009AC530(void*, void*)
{
    ++g_destroyCalls;
}

int main()
{
    const bool passed =
        CheckExistingEqualPointer() &&
        CheckExistingOrdered() &&
        CheckInsert(true) &&
        CheckInsert(false);
    if (!passed)
    {
        printf("FABLETLC_CONTAINED_BANK_MAP_INDEX FAIL\n");
        return 1;
    }

    printf("FABLETLC_CONTAINED_BANK_MAP_INDEX PASS\n");
    return 0;
}
