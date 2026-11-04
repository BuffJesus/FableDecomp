#include "fable_string.h"

void CCharString::SetAsSubString(const char* text, long length)
{
    if (storage_) UnassignString();
    if (text && *text) storage_ = AllocStringData(text, length);
}
