#include "fable_basic_string.h"

template<>
bool CBasicString<char>::operator==(const char* other) const
{
    const char* p = other;
    const char* base = PData - (int)p;

    int result;
    for (;;)
    {
        char a = base[(int)p];
        if (a == 0 && *p == 0)
        {
            result = 0;
            break;
        }
        char b = *p;
        if (a < b)
        {
            result = -1;
            break;
        }
        if (a > b)
        {
            result = 1;
            break;
        }
        ++p;
    }
    return result == 0;
}
