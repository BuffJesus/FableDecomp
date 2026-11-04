#include "fable_counted_pointer.h"
class CShaderResource;

// CCountedPointer<CShaderResource>::ShareData: out-of-line COMDAT emitted in the
// CRenderManager2D TU (its inline twin is operator= 0x009DE7C0, called from
// CRenderManager2D::CRenderManager2D 0x009D9B88 to assign VertexShaderResource).
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

template void CCountedPointer<CShaderResource>::ShareData(CShaderResource*, CCPPointerInfo*);
