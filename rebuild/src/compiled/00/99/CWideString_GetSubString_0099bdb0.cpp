// Typed CWideString::GetSubString @ 0099BDB0; second index is exclusive end.
// Inline string-family lifetime/substring bodies preserve the retail ABI.
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


inline CWideStringData* CWideString::AllocStringData(const wchar_t* text,long length){if(length==-1)return new CWideStringData(text);return new CWideStringData(text,length);}
inline CWideString::CWideString():storage_(0){++g_CWideStringInstanceCount_013BCA20;}
inline const wchar_t* CWideString::GetAsCharArray()const{if(storage_==0)return g_FableEmptyWideString_0129A8E0;return storage_->text;}
__forceinline void CWideString::SetAsSubString(const wchar_t* text,long length){UnassignString();if(text&&*text)storage_=AllocStringData(text,length);}
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


CWideString CWideString::GetSubString(unsigned long start,unsigned long end)const {
 CWideString result;
 result.SetAsSubString(GetAsCharArray()+start,end-start);
 return result;
}
