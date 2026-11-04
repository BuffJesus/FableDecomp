#pragma optimize("s",on)
#include "engine/CFontBank.h"
#include "fable_counted_pointer.h"
template<class T> void CCountedPointer<T>::Reset(T* data)
{
    ReleaseData();
    Data = data;
    if (Data) Info = new CCPPointerInfo(DeleteData, Data);
}
template void CCountedPointer<CFontBank>::Reset(CFontBank*);
