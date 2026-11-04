#include "fable_string.h"
inline const wchar_t* CWideString::GetAsCharArray() const {
    if(storage_==0)return g_FableEmptyWideString_0129A8E0;
    return storage_->text;
}
bool CWideString::operator<(const CWideString& other) const {
    return wcscmp(GetAsCharArray(),other.GetAsCharArray())<0;
}
