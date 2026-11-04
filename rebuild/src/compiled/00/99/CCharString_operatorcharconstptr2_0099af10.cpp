// CWideString::GetAsCharArray() const @ 0x0099AF10 (FableWin
// ?GetAsCharArray@CWideString@@QBEPB_WXZ): the text, or the shared empty
// string when the string has no data. operator const wchar_t* 0x0099AF00 is
// the same body; CAFile::ExpandPath 0x00998C90 calls both (Ego_r order).
#include "fable_string.h"

const wchar_t* CWideString::GetAsCharArray() const
{
    if (storage_ == 0)
        return g_FableEmptyWideString_0129A8E0;
    return storage_->text;
}
