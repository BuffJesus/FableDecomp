// operator+(CWideString const&, wchar_t const*) @ 0x0099BF30.
#include "fable_string.h"

#include <stdlib.h>

// Retail compiles these members with operator+ in one unit; seeing their
// bodies lets VC7.1 keep the temporary out of memory across the calls.
// The helpers shared with the other operator+ units are inline (COMDAT
// pick-any) so the objects link together; their code is unchanged.
inline void CWideString::UnassignString()
{
    CWideStringData* storage = storage_;
    if (storage != 0)
    {
        if (--storage->owners <= 0)
        {
            storage = storage_;
            if (storage != 0)
            {
                if (storage->text != 0)
                    free(storage->text);
                delete storage;
            }
        }
        storage_ = 0;
    }
}

inline CWideStringData* CWideString::AllocStringData(
    const wchar_t* text,
    long length)
{
    CWideStringData* data;
    if (length == -1)
        data = new CWideStringData(text);
    else
        data = new CWideStringData(text, length);
    return data;
}

inline void CWideString::MakeOwnCopyOfData()
{
    if (storage_ != 0 && storage_->owners > 1)
    {
        CWideStringData* copy = new CWideStringData(storage_->Data.c_str());
        UnassignString();
        storage_ = copy;
    }
}

const CWideString& CWideString::operator+=(const wchar_t* text)
{
    if (storage_ != 0)
    {
        MakeOwnCopyOfData();
        storage_->Data.append(text);
    }
    else if (text != 0 && *text != L'\0')
    {
        storage_ = AllocStringData(text, -1);
    }
    return *this;
}

inline CWideString::CWideString(const CWideString& other)
    : storage_(0)
{
    ++g_CWideStringInstanceCount_013BCA20;

    if (&other != this && other.storage_ != storage_)
    {
        if (storage_ != 0)
            UnassignString();

        if (other.storage_ != 0)
        {
            storage_ = other.storage_;
            ++storage_->owners;
        }
    }
}

inline CWideString::~CWideString()
{
    UnassignString();
    --g_CWideStringInstanceCount_013BCA20;
}

CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const wchar_t* right)
{
    CWideString result(left);
    result += right;
    return result;
}
