// CCountedPointer<T>::DeleteData for the mesh-file chunk pointers (retail 0x00A9CB20, 0x00A9CB50, 0x00AC1460,
// 0x00ACF3B0..0x00ACF440): the shared block's delete function - a virtual delete of the chunk.
#include "fable_chunked_file.h"

template void CCountedPointer<C3DMeshFileGroupChunk>::DeleteData(void*);
template void CCountedPointer<C3DMeshFileVerticesChunk>::DeleteData(void*);
template void CCountedPointer<C3DMeshFileTrianglesChunk>::DeleteData(void*);
template void CCountedPointer<C3DMeshFilePrimitiveChunk>::DeleteData(void*);
template void CCountedPointer<C3DMeshFileRootChunk>::DeleteData(void*);
template void CCountedPointer<C3DMeshFileMapChunk>::DeleteData(void*);
template void CCountedPointer<C3DMeshFile>::DeleteData(void*);
