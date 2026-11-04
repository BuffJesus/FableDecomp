// CWideString::GetLength @ 0x0099B220 (FableWin ?GetLength@CWideString@@QBEJXZ).
//
// Character count: the text vector's last minus first (0 when empty).

#include "fable_string.h"

long CWideString::GetLength() const
{
    if (storage_ == 0)
        return 0;
    return reinterpret_cast<const wchar_t*>(storage_->unknown04) - storage_->text;
}
