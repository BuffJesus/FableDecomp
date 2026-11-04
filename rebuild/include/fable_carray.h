#pragma once
#include "fable_stlport_vector.h"

// FableWin's CArray<T> in its retail shape: an STLport vector (12 bytes) plus
// the raw-pointer accessors. CArrayVector retains the established spelling
// used by earlier recoveries; CArray below has the original symbol identity.
template <class T>
class CArrayVector : public _STL::vector<T>
{
public:
    CArrayVector()
    {
    }
    // Ego_r CArray<T>(n) in its STLport shape: n value-initialised elements
    // (CDataInputStream::CopyTo 0x009964E0's chunk buffer).
    explicit CArrayVector(size_t count)
        : _STL::vector<T>(count)
    {
    }

    // Retail 009E7680 for symbol entries: release storage, preserving derived map flags.
    void DeallocateMemory();
    void DeallocateUnusedMemory();

    // FableWin ?GetAsCArray@?$CArray@D@@QBEPBDXZ (inline in retail): the
    // elements, or the array object itself while it is empty. The size test
    // is a subtraction (`sub`), where empty() would be a compare.
    const T* GetAsCArray() const
    {
        if (this->size() == 0)
            return GetZeroSizeArray();
        return &(*this)[0];
    }
    // FableWin ?GetAsCArray@?$CArray@E@@QAEPAEXZ (the non-const form;
    // CDefinitionManager::LoadBinaryDefinitions 0x009AFB90 reads into it).
    // The test is `size() > 0` with the element pointer as the default
    // (retail: `test; ja` over the start, CDataInputStream::CopyTo 0x009964E0).
    T* GetAsCArray()
    {
        if (this->size() > 0)
            return &(*this)[0];
        return const_cast<T*>(GetZeroSizeArray());
    }

private:
    // FableWin ?GetZeroSizeArray@?$CArray@D@@ABEPBDXZ.
    const T* GetZeroSizeArray() const { return reinterpret_cast<const T*>(this); }
};

// Original CArray identity for newly recovered method signatures. Keep the
// established CArrayVector spelling above until its existing callers migrate.
template <class T>
class CArray : public _STL::vector<T>
{
public:
    CArray()
    {
    }
    // Ego_r CArray<T>(n) in its STLport shape: n value-initialised elements
    // (CDataInputStream::CopyTo 0x009964E0's chunk buffer).
    explicit CArray(size_t count)
        : _STL::vector<T>(count)
    {
    }

    // Retail 009E7680 for symbol entries: release storage, preserving derived map flags.
    void DeallocateMemory();
    void DeallocateUnusedMemory();

    // Ego_r ?Sizeof@?$CArray@...@@QBEKXZ (inline in retail): the array object plus its whole
    // allocation (CPhysicsFacesSegmentLeaf::Sizeof 0x00A86100: `(eos - start) >> 1`, `lea [eax+eax+0xC]`;
    // FableWin CArray<long>::Sizeof is `capacity * 4 + sizeof`).
    unsigned long Sizeof() const
    {
        return sizeof(*this) + this->capacity() * sizeof(T);
    }

    // FableWin ?GetAsCArray@?$CArray@D@@QBEPBDXZ (inline in retail): the
    // elements, or the array object itself while it is empty. The test is
    // `size() > 0` (a subtraction; for elements wider than a byte retail
    // shows `sar; test; ja`, C3DMeshFileXTriangleStripChunk::WriteToFile
    // 0x00AA5930), where empty() would be a compare.
    const T* GetAsCArray() const
    {
        if (this->size() > 0)
            return &(*this)[0];
        return GetZeroSizeArray();
    }
    // FableWin ?GetAsCArray@?$CArray@E@@QAEPAEXZ (the non-const form;
    // CDefinitionManager::LoadBinaryDefinitions 0x009AFB90 reads into it).
    // The test is `size() > 0` with the element pointer as the default
    // (retail: `test; ja` over the start, CDataInputStream::CopyTo 0x009964E0).
    T* GetAsCArray()
    {
        if (this->size() > 0)
            return &(*this)[0];
        return const_cast<T*>(GetZeroSizeArray());
    }

private:
    // FableWin ?GetZeroSizeArray@?$CArray@D@@ABEPBDXZ.
    const T* GetZeroSizeArray() const { return reinterpret_cast<const T*>(this); }
};
