#include "fable_def_string.h"
CCharString CDefStringTable::GetString(long tablePos) const
{
    if (tablePos == -1) return CCharString();
    const char* text = static_cast<const char*>(StringStream.PeekData()) + tablePos + 4;
    return CCharString(text);
}