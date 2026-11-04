#pragma once
#include "fable_resource_list.h"

// Shared intrusive pointer, moved verbatim from fable_game_text_bank.h.
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
        if (data_)
            data_->AddRef();
    }

    // CIVCountedPointer<const T> from CIVCountedPointer<T>: shares and adds a reference.
    template<typename U>
    CIVCountedPointer(const CIVCountedPointer<U>& other)
        : data_(other.data_)
    {
        if (data_)
            data_->AddRef();
    }

    ~CIVCountedPointer()
    {
        if (data_ != 0)
        {
            static_cast<const CIVCountedPointeeBase*>(data_)->Release();
            data_ = 0;
        }
    }

    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (data_ != other.data_)
        {
            if (data_)
            {
                static_cast<const CIVCountedPointeeBase*>(data_)->Release();
                data_ = 0;
            }
            data_ = other.data_;
            if (data_)
                data_->AddRef();
        }
        return *this;
    }

    void Release()
    {
        if (data_)
        {
            static_cast<const CIVCountedPointeeBase*>(data_)->Release();
            data_ = 0;
        }
    }
    void Reset(T* value = 0)
    {
        Release();
        data_ = value;
        if (data_) data_->AddRef();
    }

    bool IsNull() const
    {
        return data_ == 0;
    }

    T* operator->() const
    {
        return data_;
    }

    T* data_;
};
