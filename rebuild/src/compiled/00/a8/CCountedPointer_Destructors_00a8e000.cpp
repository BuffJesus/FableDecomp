// Out-of-line CCountedPointer<T> destructors retail keeps as COMDATs (0x00A8E000, 0x00A94C70, 0x00A9A350, 0x00AAC870,
// 0x00A9A3E0, 0x00A9CE90, 0x00AC1A90 - each follows the CCountedPointer<T>(T*) constructor installing its DeleteData):
// the destructor only releases the shared block, so the pointee types stay incomplete here.
#include "fable_counted_pointer.h"
#include "fable_soundemitter_lh.h"

class CTriangleStripifierBase;
class C3DMeshPartitionElement;
class CChunkedFileSuperChunk;
class C3DAnimFileRootChunk;
class C3DMeshFileRootChunk;
class C3DMeshFileMapChunk;

template CCountedPointer<CTriangleStripifierBase>::~CCountedPointer();
template CCountedPointer<C3DMeshPartitionElement>::~CCountedPointer();
template CCountedPointer<CChunkedFileSuperChunk>::~CCountedPointer();
template CCountedPointer<CSoundEmitterLH::CActiveSound>::~CCountedPointer();
template CCountedPointer<C3DAnimFileRootChunk>::~CCountedPointer();
template CCountedPointer<C3DMeshFileRootChunk>::~CCountedPointer();
template CCountedPointer<C3DMeshFileMapChunk>::~CCountedPointer();
