#include "fable_string.h"

CCharString::CCharString(CAFile* file, long length)
    : storage_(0)
{
    ++g_CCharStringInstanceCount_013BD800;
    if (storage_ != 0)
        UnassignString();
    LoadFromFile(*file, 0, length);
}
