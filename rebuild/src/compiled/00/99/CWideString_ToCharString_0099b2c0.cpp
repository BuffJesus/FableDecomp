#include "fable_string.h"
#include "fable_carray.h"
#include "fable_stlport_vector_impl.h"
inline long CWideString::GetLength() const
{
    if (storage_ == 0) return 0;
    return reinterpret_cast<const wchar_t*>(storage_->unknown04) - storage_->text;
}
inline const wchar_t* CWideString::GetAsCharArray() const
{
    return storage_ == 0 ? g_FableEmptyWideString_0129A8E0 : storage_->text;
}
CCharString CWideString::ToCharString() const
{
    CArrayVector<char> bytes(GetLength() + 1);
    for (long i = 0; i < GetLength(); ++i)
        bytes[i] = static_cast<char>(GetAsCharArray()[i]);
    bytes[GetLength()] = 0;
    return CCharString(&bytes[0]);
}
