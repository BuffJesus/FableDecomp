#include <stdio.h>

#include "fable_string.h"

wchar_t g_FableEmptyWideString_0129A8E0[1] = {0};

int main()
{
    fable_u8 objectBytes[sizeof(CWideString)];
    CWideString* value = reinterpret_cast<CWideString*>(objectBytes);

    *reinterpret_cast<CWideStringData**>(value) = 0;
    if (static_cast<const wchar_t*>(*value) != g_FableEmptyWideString_0129A8E0)
        return 1;

    wchar_t text[] = L"names.bin";
    unsigned long storageRaw_[4];
    CWideStringData& storage = *reinterpret_cast<CWideStringData*>(storageRaw_);
    storage.text = text;
    storage.unknown04 = 0;
    storage.unknown08 = 0;
    storage.owners = 1;
    *reinterpret_cast<CWideStringData**>(value) = &storage;

    if (static_cast<const wchar_t*>(*value) != text)
        return 2;

    puts("FABLETLC_CWIDESTRING_OPERATOR_CONST_WCHAR_0099AF00_PASS");
    return 0;
}
