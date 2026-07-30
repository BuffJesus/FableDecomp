#include "fable_string.h"

CWideString::operator const wchar_t*() const
{
    if (storage_ == 0)
        return g_FableEmptyWideString_0129A8E0;
    return storage_->text;
}
