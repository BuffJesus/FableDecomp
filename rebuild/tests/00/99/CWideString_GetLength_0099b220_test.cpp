// Behaviour fixture for CWideString::GetLength (0x0099B220).
// Links the compiled source object.
#define private public
#include "fable_string.h"
#undef private

#include <stdio.h>
#include <string.h>

int main()
{
    int fail = 0;
    unsigned long raw[1] = { 0 };
    const CWideString* s = reinterpret_cast<const CWideString*>(raw);

    // Empty string: 0.
    if (s->GetLength() != 0) fail |= 1;

    // Characters between first and last (not the capacity end, not the owners).
    wchar_t text[16] = L"Albion";
    unsigned long dataRaw_[4];
    CWideStringData& data = *reinterpret_cast<CWideStringData*>(dataRaw_);
    data.text = text;
    data.unknown04 = reinterpret_cast<fable_u32>(text + 6);
    data.unknown08 = reinterpret_cast<fable_u32>(text + 16);
    data.owners = 3;
    raw[0] = reinterpret_cast<unsigned long>(&data);
    if (s->GetLength() != 6) fail |= 2;
    data.unknown04 = reinterpret_cast<fable_u32>(text);
    if (s->GetLength() != 0) fail |= 4;
    data.unknown04 = reinterpret_cast<fable_u32>(text + 15);
    if (s->GetLength() != 15) fail |= 8;
    if (data.owners != 3 || data.text != text) fail |= 16;

    if (fail == 0)
        printf("CHARSTRING_GETLENGTH_OK\n");
    else
        printf("CHARSTRING_GETLENGTH_FAIL %d\n", fail);
    return 0;
}
