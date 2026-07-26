#include "fable_string.h"

CCharString::CCharString()
    : storage_(0)
{
    ++g_CCharStringInstanceCount_013BD800;
}
