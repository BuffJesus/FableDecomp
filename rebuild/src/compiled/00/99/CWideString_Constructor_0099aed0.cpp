#include "fable_filesystem.h"

CWideString::CWideString()
    : storage_(0)
{
    ++g_CStringInstanceCount_013BCA20;
}
