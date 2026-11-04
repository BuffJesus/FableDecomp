#include "engine/CFontBank.h"
#include "fable_counted_pointer.h"
template<class T> void CCountedPointer<T>::ReleaseData()
{
    if (Info)
    {
        --Info->RefCount;
        if (Info->RefCount == 0)
        {
            Info->DeleteFunc(Info->Data);
            delete Info;
        }
    }
    Data = 0;
    Info = 0;
}
template void CCountedPointer<CFontBank>::ReleaseData();
