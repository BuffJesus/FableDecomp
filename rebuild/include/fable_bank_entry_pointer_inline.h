#pragma once
#include "fable_counted_pointer.h"
class CBankFileAsyncEntry;
// Opt-in bodies for retail callers which inline entry ownership operations.
// Do not combine with fable_bank_entry_pointer_outofline.h in one TU.
template<> inline CCountedPointer<CBankFileAsyncEntry>::CCountedPointer(const CCountedPointer& other)
    : Data(other.Data),Info(other.Info)
{
    if (Info) ++Info->RefCount;
}
template<> inline CCountedPointer<CBankFileAsyncEntry>& CCountedPointer<CBankFileAsyncEntry>::operator=(const CCountedPointer& other)
{
    CBankFileAsyncEntry* data=other.Data;
    CCPPointerInfo* info=other.Info;
    if (Info!=info) {
        Release();
        Data=data;
        Info=info;
        if (Info) ++Info->RefCount;
    }
    return *this;
}
