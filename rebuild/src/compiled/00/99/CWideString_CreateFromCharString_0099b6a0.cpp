#include "fable_string.h"

CWideString FABLE_FASTCALL CWideString::CreateFromCharString(
    const CCharString& text)
{
    return CWideString(text);
}
