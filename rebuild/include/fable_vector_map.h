#pragma once

#include <stddef.h>

#include "rebuild_abi.h"

// Lionhead's sorted-vector map. Retail layout (16 bytes): the STLport
// vector of pairs (Ego_r's CArray<pair<K,V>> base, 12 bytes in retail),
// then the empty comparator (+0x0C) and the Dirty flag (+0x0D). Only the
// inline ctor is modelled (CDefinitionManager 0x009B0470 zeroes the vector
// and stores Dirty = false for its three maps).
#include "fable_stlport_tree.h"
#include "fable_stlport_algo.h"
#include "fable_carray.h"

// Lionhead's key-order comparator for CVectorMap entries (empty); inline in
// retail's sort helpers (<unsigned long, CDefinitionManager::CDefClassInfo>:
// __introsort_loop 0x009B9320).
template <class K, class V>
class CKeyPairCompareLess
{
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const
    {
        return a.first < b.first;
    }
};

// Ego_r: CVectorMap<K,V,C> : CArray<pair<K,V> > (CArrayVector here; its
// GetAsCArray is what CDefinitionManager::LoadBinaryDefinitions 0x009AFB90
// reads the compressed offsets into).
template <class K, class V, class C>
class CVectorMap : public CArrayVector<_STL::pair<K, V> >
{
public:
    CVectorMap()
        : Dirty(false)
    {
    }

    // FableWin ?Insert@?$CVectorMap@...@@QAEXABV...@ABV...@@Z; inline in
    // retail (CSymbolMap::AddSymbolDirty 0x009E54E0): append unsorted and
    // leave the sort to the next Update.
    __forceinline void Insert(const K& key, const V& value)
    {
        this->push_back(_STL::pair<K, V>(key, value));
        Dirty = true;
    }

    // FableWin ?Sort@?$CVectorMap@...@@QAEXXZ: sort by key unconditionally and mark the map clean. Out of line in
    // retail for the nav maps (<unsigned __int64, bool>: 0x00A84590, <unsigned __int64, CANavQuadTreeNode*>: 0x00A845E0).
    void Sort()
    {
        _STL::sort(this->begin(), this->end(), Compare);
        Dirty = false;
    }

    // FableWin ?Update@?$CVectorMap@...@@QAEXXZ: sort only when entries were appended since the last sort. Inline in
    // retail (CSymbolMap::UpdateDirty 0x009E5C30); out of line for the nav maps (0x00A84B10, 0x00A84B60). Retail
    // repeats the sort here rather than calling Sort.
    void Update()
    {
        if (Dirty)
        {
            _STL::sort(this->begin(), this->end(), Compare);
            Dirty = false;
        }
    }

    // FableWin ?LowerBound@?$CVectorMap@...@@QAE?AV?$_Vector_iterator@...@@ABK@Z
    // (and the const overload): binary search for the first entry whose key
    // is not below `key`. Out of line in retail (<unsigned long,
    // CDefinitionManager::CDefClassInfo>: 0x009B2230, const 0x009B2290).
    _STL::pair<K, V>* LowerBound(const K& key);
    const _STL::pair<K, V>* LowerBound(const K& key) const;
    // First entry strictly above key; compressed-block lookup, 0x009B1FF0;
    // the non-const overload (<unsigned long, CSubDefInfo>: 0x009B2190) bounds
    // EraseFromKey's copy.
    _STL::pair<K, V>* UpperBound(const K& key);
    const _STL::pair<K, V>* UpperBound(const K& key) const;

    // FableWin ?EraseFromKey@?$CVectorMap@...@@QAEXABK@Z; inline in retail
    // (CParentDefClassBase::RemoveSubDef 0x009B0170): drop every entry under
    // `key` (FableWin: LowerBound, compare with end(), then erase up to
    // UpperBound). Retail moves the tail down with the out-of-line copy and
    // reads the old end before calling UpperBound, so the bound is taken in
    // the copy's own arguments; the entries need no destruction.
    void EraseFromKey(const K& key)
    {
        _STL::pair<K, V>* it = LowerBound(key);
        if (it != this->end())
            this->_M_finish = _STL::copy(UpperBound(key), this->end(), it);
    }

    // FableWin ?Find@?$CVectorMap@...@@QAE?AV?$_Vector_iterator@...@@ABK@Z;
    // inline in retail (CDefinitionManager::GetPDefClassInfo 0x009AD580):
    // the entry with exactly this key, or end(). The search gets its own
    // copy of the key: retail passes the address of a spilled copy and keeps
    // comparing the original in a register.
    _STL::pair<K, V>* Find(const K& key)
    {
        const K searchKey = key;
        _STL::pair<K, V>* it = LowerBound(searchKey);
        if (it == this->end() || key < it->first)
            return this->end();
        return it;
    }
    // The const overload (inline in retail: the const GetPDefClassInfo
    // 0x009AD2E0 calls the const LowerBound 0x009B2290 the same way).
    const _STL::pair<K, V>* Find(const K& key) const
    {
        const _STL::pair<K, V>* it = LowerBound(key);
        if (it == this->end() || key < it->first)
            return this->end();
        return it;
    }

    // Ego_r CArray<pair<K,V> >::DeallocateUnusedMemory: shrink the storage to
    // the size. It is declared on the map because fable_game_text_bank.h's
    // CArray still carries the 16-byte Ego_r shape; out of line in retail
    // (<CCharString, CSymbolMap::CSymbolInfo>: 0x009E7C70).
    void DeallocateUnusedMemory();

    // CVectorMap reset (symbol entries: 009E79E0) also clears Dirty.
    void DeallocateMemory();

    C Compare;
    bool Dirty;
};

FABLE_STATIC_ASSERT(sizeof(CVectorMap<unsigned long, long, CKeyPairCompareLess<unsigned long, long> >) == 0x10);
