// CCountedPointer<T>::DeleteData for the anim-file chunk pointers (retail 0x00A99FD0 CChunkedFileSuperChunk,
// 0x00A99FE0 C3DAnimFileRootChunk, 0x00A99FF0 CChunkedFileChunk, 0x00A9A000 C3DAnimFile; the lib_3d_anim_file template
// pool, Ego_r/FableWin CU order): the shared block's delete function - a virtual delete of the chunk. The landed
// CCountedPointer<C3DAnimFileRootChunk>::Reset 0x00A9A420 and <CChunkedFileChunk>::Reset 0x00A9CF40 store these.
#include "fable_chunked_file.h"

template void CCountedPointer<CChunkedFileSuperChunk>::DeleteData(void*);
template void CCountedPointer<C3DAnimFileRootChunk>::DeleteData(void*);
template void CCountedPointer<CChunkedFileChunk>::DeleteData(void*);
template void CCountedPointer<C3DAnimFile>::DeleteData(void*);
