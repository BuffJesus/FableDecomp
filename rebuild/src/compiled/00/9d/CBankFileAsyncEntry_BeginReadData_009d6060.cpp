#include "fable_bank_async_entry.h"

// Same counted-pointer template context as the BeginReadDataAutoAlloc TU (009D6100).
template<class T> CCountedPointer<T>::CCountedPointer(T* data)
    : Data(data), Info(data ? new CCPPointerInfo(DeleteData, data) : 0) {}

template<class T> CCountedPointer<T>::CCountedPointer(const CCountedPointer<T>& other)
    : Data(other.Data), Info(other.Info)
{
    if (Info) ++Info->RefCount;
}

// Retail 009D6060: caller-supplied buffer, so no alignment skip or pool.
CCountedPointer<CBankFileAsyncData> CBankFileAsyncEntry::BeginReadData(
    unsigned long offset, void* buffer, unsigned long size, unsigned long bufferSize,
    EThreadedFilePriority priority)
{
    CCountedPointer<CBankFileAsyncData> data(new CBankFileAsyncData(buffer, size, BankFile));
    SetupRead(offset, *data, size, bufferSize, 0, priority);
    return data;
}
