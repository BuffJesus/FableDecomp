#pragma once
// Opt-in inline bodies for CCountedPointer members that retail expands in the
// units including this header (lib_chunked_file.cpp and the mesh-file chunk
// finders). Elsewhere these stay out of line as per-T COMDATs, so the main
// header only declares them.
#include "fable_counted_pointer.h"

// Copying shares the data.
template<typename T>
inline CCountedPointer<T>::CCountedPointer(const CCountedPointer& other)
    : Data(other.Data), Info(other.Info)
{
    if (Info)
        ++Info->RefCount;
}

// Taking ownership of new data (FableWin: Info set on both branches).
template<typename T>
inline CCountedPointer<T>::CCountedPointer(T* data)
    : Data(data)
{
    if (Data)
        Info = new CCPPointerInfo(&DeleteData, (void*)Data);
    else
        Info = 0;
}

// A CCountedPointer<const T> from a CCountedPointer<T>.
template<typename T>
template<typename U>
inline CCountedPointer<T>::CCountedPointer(const CCountedPointer<U>& other)
    : Data(other.Data), Info(other.Info)
{
    if (Info)
        ++Info->RefCount;
}

// FableWin ?ReleaseData@...@@QAEXXZ: drop the reference, then clear both
// fields (out of line elsewhere as per-T COMDATs, e.g. <CFontBank> 0x004057A0).
template<typename T>
inline void CCountedPointer<T>::ReleaseData()
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

// FableWin ?ShareData@...@@QAEXPAV...@PAVCCPPointerInfo@@@Z.
template<typename T>
inline void CCountedPointer<T>::ShareData(T* data, CCPPointerInfo* info)
{
    if (Info != info)
    {
        ReleaseData();
        Data = data;
        Info = info;
        if (Info)
            ++Info->RefCount;
    }
}

// FableWin ??4?$CCountedPointer@...@@QAEAAV0@ABV0@@Z: share the other's data
// (both fields read before the release; the debug build then asserts).
template<typename T>
inline CCountedPointer<T>& CCountedPointer<T>::operator=(const CCountedPointer& other)
{
    ShareData(other.Data, other.Info);
    return *this;
}

// FableWin ??$SetStaticCast@V...@@@?$CCountedPointer@V...@@@@QAEXABV?$CCountedPointer@V...@@@@@Z:
// share another pointer's data as a T.
template<typename T>
template<typename U>
inline void CCountedPointer<T>::SetStaticCast(const CCountedPointer<U>& other)
{
    ShareData(static_cast<T*>(other.Data), other.Info);
}
