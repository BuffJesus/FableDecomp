#include "fable_string.h"

inline CCharString::~CCharString()
{
    UnassignString();
    --g_CCharStringInstanceCount_013BD800;
}

void CCharString::RemoveWhiteSpaceBegin()
{
    unsigned long first = 0;
    for (;;)
    {
        if (first >= GetLength())
            return;
        char value = GetConstCharPtr()[first];
        if (value != ' ' && value != '\t' && value != '\r' && value != '\n')
            break;
        ++first;
    }
    if (first > 0)
    {
        CCharString trimmed = GetSubString(first, GetLength());
        if (&trimmed != this)
        {
            if (storage_ != 0)
                UnassignString();
            if (trimmed.storage_ != 0)
            {
                storage_ = trimmed.storage_;
                ++storage_->owners;
            }
        }
    }
}
