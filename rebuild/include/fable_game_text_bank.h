#pragma once

#include "rebuild_abi.h"

template<typename T>
class CIVCountedPointer
{
public:
    CIVCountedPointer()
        : data_(0)
    {
    }

    CIVCountedPointer(const CIVCountedPointer& other)
        : data_(other.data_)
    {
    }

    ~CIVCountedPointer()
    {
    }

    T* data_;
};

namespace NGameText
{
    class CGroupEntry;

    class CDataBank
    {
    public:
        CDataBank();

        CIVCountedPointer<const CGroupEntry> GetPGroupEntry(
            unsigned long index,
            bool allowPending) const;

    private:
        fable_u8 storage_[0x214];
    };
}
