#include "fable_string.h"

inline CCharString::~CCharString()
{
    UnassignString();
    --g_CCharStringInstanceCount_013BD800;
}

void CCharString::RemoveWhiteSpaceEnd()
{
    unsigned long last = GetLength();
    while (last > 0)
    {
        char value = GetConstCharPtr()[last - 1];
        if (value != ' ' && value != '\t' && value != '\r' && value != '\n')
            break;
        --last;
        if (last == 0)
            return;
    }
    if (last < GetLength())
    {
        CCharString trimmed = GetSubString(0, last);
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
