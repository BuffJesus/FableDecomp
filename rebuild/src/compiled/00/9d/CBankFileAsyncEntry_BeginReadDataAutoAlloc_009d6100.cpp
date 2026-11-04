#include "fable_bank_async_entry.h"

// Genuine template context already present in CBankFileAsync_GetAsyncEntry TU.
template<class T> CCountedPointer<T>::CCountedPointer(T* data)
    : Data(data), Info(data ? new CCPPointerInfo(DeleteData, data) : 0) {}

template<class T> CCountedPointer<T>::CCountedPointer(const CCountedPointer<T>& other)
    : Data(other.Data), Info(other.Info)
{
    if (Info) ++Info->RefCount;
}

CCountedPointer<CBankFileAsyncData> CBankFileAsyncEntry::BeginReadDataAutoAlloc(
    unsigned long offset, unsigned long size, EThreadedFilePriority priority)
{
    unsigned long alignedOffset, skip;
    if (BankFile.NonCached)
    {
        unsigned long alignment = BankFile.GetAlignment();
        alignedOffset = (offset / alignment) * alignment;
        skip = offset - alignedOffset;
    }
    else
    {
        alignedOffset = offset;
        skip = 0;
    }
    unsigned long allocationSize = BankFile.Align(size + skip);
    CCountedPointer<CBankFileAsyncData> data(new CBankFileAsyncData(
        size, skip, BankFile.LoadingMemoryPool.Data, BankFile));
    SetupRead(alignedOffset, *data, size + skip, allocationSize, skip, priority);
    return data;
}
