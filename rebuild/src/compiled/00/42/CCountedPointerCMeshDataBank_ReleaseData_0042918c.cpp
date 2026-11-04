#pragma optimize("s",on)
#include "fable_counted_pointer.h"
class CMeshDataBank;

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

template void CCountedPointer<CMeshDataBank>::ReleaseData();
template void CCountedPointer<CMeshDataBank>::ShareData(CMeshDataBank*, CCPPointerInfo*);
