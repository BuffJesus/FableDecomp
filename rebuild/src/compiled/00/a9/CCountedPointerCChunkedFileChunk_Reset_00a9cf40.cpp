// CCountedPointer<CChunkedFileChunk>::Reset, retail 0x00A9CF40: the per-T
// COMDAT that C3DMeshFileLoader::AddChunkType (0x00A9AD00) calls; and the
// <C3DAnimFileRootChunk> (0x00A9A420) and <C3DMeshFileRootChunk> (0x00A9CED0)
// ones from the two loader constructors.
#include "fable_chunked_file.h"

template<typename T>
void CCountedPointer<T>::Reset(T* data)
{
    Release();
    Info = 0;
    Data = data;
    if (Data)
        Info = new CCPPointerInfo(&DeleteData, Data);
}

template void CCountedPointer<CChunkedFileChunk>::Reset(CChunkedFileChunk* data);
template void CCountedPointer<C3DAnimFileRootChunk>::Reset(C3DAnimFileRootChunk* data);
template void CCountedPointer<C3DMeshFileRootChunk>::Reset(C3DMeshFileRootChunk* data);
