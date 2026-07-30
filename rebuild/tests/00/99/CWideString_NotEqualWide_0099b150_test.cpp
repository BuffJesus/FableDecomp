#include <stdio.h>
#include <string.h>

#include "fable_string.h"

extern const wchar_t g_FableEmptyWideComparison_0122D70C[] = L"";

int FABLE_CDECL MemCmp_Unsigned16(
    const unsigned short* left,
    const unsigned short* right,
    unsigned long count)
{
    for (unsigned long index = 0; index != count; ++index)
    {
        if (left[index] != right[index])
            return left[index] < right[index] ? -1 : 1;
    }
    return 0;
}

namespace
{
    CWideString& MakeValue(
        CWideStringData*& slot,
        CWideStringData* storage)
    {
        slot = storage;
        return *reinterpret_cast<CWideString*>(&slot);
    }

    void SetText(CWideStringData& storage, wchar_t* text)
    {
        memset(&storage, 0, sizeof(storage));
        storage.text = text;
        storage.unknown04 =
            reinterpret_cast<fable_u32>(text + wcslen(text));
    }
}

int main()
{
    CWideStringData* slot = 0;
    CWideString& empty = MakeValue(slot, 0);
    if (empty != L"" || !(empty != L"Fable"))
        return 1;

    wchar_t fable[] = L"Fable";
    CWideStringData storage;
    SetText(storage, fable);
    CWideString& value = MakeValue(slot, &storage);
    if (value != L"Fable")
        return 2;
    if (!(value != L"Fab") || !(value != L"Fable TLC"))
        return 3;
    if (!(value != L"fable"))
        return 4;

    puts("FABLETLC_WIDE_STRING_NOT_EQUAL_BEHAVIOR PASS");
    return 0;
}
