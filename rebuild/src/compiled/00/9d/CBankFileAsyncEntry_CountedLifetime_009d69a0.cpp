#include "fable_bank_entry_pointer_outofline.h"

template<> inline CCountedPointer<CBankFileAsyncEntry>::CCountedPointer(const CCountedPointer& other)
    : Data(other.Data),Info(other.Info)
{
    if(Info) ++Info->RefCount;
}

template<> __declspec(noinline) CCountedPointer<CBankFileAsyncEntry>::~CCountedPointer()
{
    Release();
    Data=0;
    Info=0;
}

template<> inline CCountedPointer<CBankFileAsyncEntry>& CCountedPointer<CBankFileAsyncEntry>::operator=(const CCountedPointer& other)
{
    CBankFileAsyncEntry* data=other.Data;
    CCPPointerInfo* info=other.Info;
    if(Info!=info) {
        Release();
        Data=data;
        Info=info;
        if(Info) ++Info->RefCount;
    }
    return *this;
}

template CCountedPointer<CBankFileAsyncEntry>::~CCountedPointer();

template CCountedPointer<CBankFileAsyncEntry>& CCountedPointer<CBankFileAsyncEntry>::operator=(const CCountedPointer&);

template CCountedPointer<CBankFileAsyncEntry>::CCountedPointer(const CCountedPointer&);
