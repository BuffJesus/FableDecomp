#include <wchar.h>

#include "fable_string.h"

extern const wchar_t g_FableEmptyWideComparison_0122D70C[];

int FABLE_CDECL MemCmp_Unsigned16(
    const unsigned short* left,
    const unsigned short* right,
    unsigned long count);

namespace
{
    bool __forceinline EqualWideText(
        const CWideStringData* storage,
        const wchar_t* other)
    {
        if (storage == 0)
            return
                wcscmp(
                    other,
                    g_FableEmptyWideComparison_0122D70C) == 0;

        long length = 0;
        while (other[length] != L'\0')
            ++length;

        const long storedLength =
            (static_cast<long>(storage->unknown04) -
                reinterpret_cast<long>(storage->text)) >> 1;
        if (
            storedLength == length &&
            MemCmp_Unsigned16(
                reinterpret_cast<const unsigned short*>(storage->text),
                reinterpret_cast<const unsigned short*>(other),
                length) == 0)
        {
            return true;
        }
        return false;
    }
}

bool CWideString::operator!=(const wchar_t* other) const
{
    return !EqualWideText(storage_, other);
}
