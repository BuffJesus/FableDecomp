#include "fable_string.h"
const CCharString& CCharString::operator=(const char* text)
{
    if (storage_ != 0) UnassignString();
    if (text != 0 && *text != 0) storage_ = new CCharStringData(text);
    return *this;
}