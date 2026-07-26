#include "fable_string.h"

CWideString CCharString::ToWideString() const
{
    CWideString result = CWideString::CreateFromCharString(*this);
    return result;
}
