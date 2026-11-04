// CWideString::operator+=(wchar_t const*) @ 0x0099B940 (FableWin
// ??YCWideString@@QAEABV0@PB_W@Z). Appends the literal to the string's own
// copy of its data (STLport basic_string<wchar_t>::append 0x0099C7E0); an
// empty string takes new data only for a non-empty literal. The length scan
// is the inline char_traits loop (STLport's append(const wchar_t*)). The
// earlier landing reached the append through a made-up five-argument helper.
#include "fable_string.h"

const CWideString& CWideString::operator+=(const wchar_t* text)
{
    if (storage_ != 0)
    {
        MakeOwnCopyOfData();
        storage_->Data.append(text);
    }
    else if (text != 0 && *text != L'\0')
    {
        storage_ = AllocStringData(text, -1);
    }
    return *this;
}
