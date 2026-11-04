#pragma optimize("s",on)
#include "fable_counted_pointer.h"
class CGraphicDataBank;
template <typename T> void CCountedPointer<T>::ReleaseData()
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
template <typename T> void CCountedPointer<T>::ShareData(T* data, CCPPointerInfo* info)
{
    if (Info != info)
    {
        ReleaseData();
        Data = data;
        Info = info;
        if (info) ++info->RefCount;
    }
}
template <typename T> CCountedPointer<T>& CCountedPointer<T>::operator=(const CCountedPointer<T>& other)
{
    ShareData(other.Data, other.Info);
    return *this;
}
template void CCountedPointer<CGraphicDataBank>::ReleaseData();
template void CCountedPointer<CGraphicDataBank>::ShareData(CGraphicDataBank*, CCPPointerInfo*);
template CCountedPointer<CGraphicDataBank>& CCountedPointer<CGraphicDataBank>::operator=(const CCountedPointer<CGraphicDataBank>&);
