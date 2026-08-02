// NBankFileManager::CContainedBankMap::operator[] @ 0x009ac530
//
// Standard MSVC7.1 std::map<CCharString, CContainedBank>::operator[] body:
// _Lbound() to find the insertion point, compare keys (interned CCharString
// data pointer identity, then a CBasicString<char>-style less-than), and on
// miss construct a default pair<CCharString, CContainedBank> on the stack and
// hand it to the tree's _Insert helper.
//
// Node layout matches std::_Tree_nod<Traits>::_Node as used elsewhere in this
// binary (see rebuild/src/compiled/00/9a/global_TreeLbound_009aad60.cpp):
//   +0x00 header (color/links, opaque here)
//   +0x04 _Parent
//   +0x08 _Left
//   +0x0c _Right
//   +0x10 _Myval.first  (CCharString key; only its interned char* is read)
//   +0x14 _Myval.second (CContainedBank, returned by reference)

#include <string.h>

#include "fable_retail_banks.h"

namespace
{
    struct Node
    {
        fable_u8 header_[0x10];
        void* keyData_;
        NBankFileManager::CContainedBank value_;
    };

    // Raw stack layout for the temporary pair<CCharString, CContainedBank>
    // handed to _Insert: key storage (4 bytes) followed by the five zeroed
    // CContainedBank dwords, matching retail's per-field zero stores.
    struct PairStorage
    {
        void* keyData_;
        fable_u32 dataVersion_;
        fable_u32 entryCount_;
        fable_u32 infoOffset_;
        fable_u32 infoSize_;
        fable_u32 alignment_;
    };

}

extern "C" Node* FABLE_FASTCALL
FableContainedBankMapLowerBound_009AC530(
    NBankFileManager::CContainedBankMap* self,
    void* /*edx*/,
    const CCharString* key);

extern "C" bool FABLE_FASTCALL
FableContainedBankMapStringLess_009AC530(
    const void* leftData,
    void* /*edx*/,
    const void* rightData);

extern "C" void FABLE_FASTCALL
FableContainedBankMapStringCopy_009AC530(
    void* destination,
    void* /*edx*/,
    const CCharString* source);

extern "C" void* FABLE_FASTCALL
FableContainedBankMapInsert_009AC530(
    NBankFileManager::CContainedBankMap* self,
    void* /*edx*/,
    void** result,
    void* hint,
    const void* value);

extern "C" void FABLE_FASTCALL
FableContainedBankMapStringDestroy_009AC530(void* string, void* /*edx*/);

NBankFileManager::CContainedBank&
NBankFileManager::CContainedBankMap::operator[](const CCharString& key)
{
    Node* whereNode = FableContainedBankMapLowerBound_009AC530(this, 0, &key);

    const void* keyData = *reinterpret_cast<void* const*>(&key);

    bool needInsert;
    if (whereNode == *reinterpret_cast<Node* const*>(this))
        needInsert = true;
    else if (whereNode->keyData_ == keyData)
        needInsert = false;
    else if (keyData == 0)
        needInsert = true;
    else if (whereNode->keyData_ == 0)
        needInsert = false;
    else
        needInsert = FableContainedBankMapStringLess_009AC530(
            keyData, 0, whereNode->keyData_);

    if (needInsert)
    {
        PairStorage pairStorage;
        FableContainedBankMapStringCopy_009AC530(
            reinterpret_cast<CCharString*>(&pairStorage.keyData_), 0, &key);

        pairStorage.dataVersion_ = 0;
        pairStorage.entryCount_ = 0;
        pairStorage.infoOffset_ = 0;
        pairStorage.infoSize_ = 0;
        pairStorage.alignment_ = 0;

        void* insertedNode = 0;
        FableContainedBankMapInsert_009AC530(
            this, 0, &insertedNode, whereNode, &pairStorage);

        FableContainedBankMapStringDestroy_009AC530(
            reinterpret_cast<CCharString*>(&pairStorage.keyData_), 0);

        whereNode = reinterpret_cast<Node*>(insertedNode);
    }

    return whereNode->value_;
}
