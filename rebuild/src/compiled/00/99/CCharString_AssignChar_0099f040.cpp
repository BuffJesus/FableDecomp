#include "fable_string.h"

CCharString& CCharString::operator=(char character)
{
    const char text[2] = { character, 0 };
    if (storage_ != 0)
        UnassignString();
    if (character != 0)
        storage_ = new CCharStringData(text);
    return *this;
}
