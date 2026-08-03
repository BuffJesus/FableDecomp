#include "fable_string.h"

CWideString::CWideString()
    : storage_(0)
{
    ++g_CWideStringInstanceCount_013BCA20;
}
