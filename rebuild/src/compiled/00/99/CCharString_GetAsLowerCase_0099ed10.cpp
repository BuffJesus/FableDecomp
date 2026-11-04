#include <new>
#include <ctype.h>
#include "fable_string.h"
inline CCharString::CCharString(const CCharString& other)
    : storage_(0)
{
    ++g_CCharStringInstanceCount_013BD800;

    if (&other != this)
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

inline void CCharString::MakeOwnCopyOfData()
{
    if (storage_ != 0 && storage_->owners > 1)
    {
        const char* text = storage_->Data.PData;
        CCharStringData* copy = new CCharStringData(text);
        UnassignString();
        storage_ = copy;
    }
}
inline void CCharString::UnassignString()
{
    CCharStringData* shared = storage_;
    if (shared != 0)
    {
        if (--shared->owners <= 0)
        {
            CCharStringData* storage = storage_;
            if (storage != 0)
            {
                if (storage->Data.PData != 0)
                {
                    delete[] storage->Data.PData;
                    storage->Data.PData = 0;
                }
                storage->Data.StringLength = 0;
                storage->Data.DataLength = 0;
                delete storage;
            }
        }
        storage_ = 0;
    }
}

inline CCharString::~CCharString()
{
    UnassignString();
    --g_CCharStringInstanceCount_013BD800;
}

CCharString CCharString::GetAsLowerCase() const {
 CCharString result(*this);
 result.MakeOwnCopyOfData();
 if(result.storage_) {
  for(long i=0;i<result.GetLength();++i) {
   result.storage_->Data.PData[i]=(char)tolower(result.storage_->Data.PData[i]);
  }
 }
 return result;
}