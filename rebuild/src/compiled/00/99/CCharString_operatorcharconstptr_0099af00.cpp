// CWideString::operator const wchar_t*() const @ 0x0099AF00 (FableWin
// ??BCWideString@@QBEPBGXZ): the text, or the shared empty string when the
// string has no data. GetAsCharArray 0x0099AF10 is the same body.
#include "fable_string.h"

CWideString::operator const wchar_t*() const
{
    if (storage_ == 0)
        return g_FableEmptyWideString_0129A8E0;
    return storage_->text;
}
