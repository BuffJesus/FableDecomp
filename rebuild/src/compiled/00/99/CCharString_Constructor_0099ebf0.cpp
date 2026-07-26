#include "fable_string.h"

CCharString::CCharString(
    const char* text,
    long length)
    : storage_(0)
{
    ++g_CCharStringInstanceCount_013BD800;

    if (storage_ != 0)
        UnassignString();

    if (text != 0 && text[0] != '\0')
        storage_ = AllocStringData(text, length);
}
