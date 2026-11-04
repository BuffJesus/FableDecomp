// CCountedPointer<const C3DMesh2>::operator=<C3DMesh2>(const CCountedPointer<C3DMesh2>&),
// retail 0x00A2ACC0 (FableWin ??$?4VC3DMesh2@@@?$CCountedPointer@$$CBVC3DMesh2@@@@QAEAAV0@ABV?$CCountedPointer@VC3DMesh2@@@@@Z).
// Callers 0x00A25EEA/0x00A25F8B/0x00A26042/0x00A26094 in C3DMeshInfo::GetMesh 0x00A25E70
// (Ego_r ?GetMesh@C3DMeshInfo@@QBE_NAAV?$CCountedPointer@$$CBVC3DMesh2@@@@KK@Z) and 0x00A2B5FD:
// each assigns a C3DMeshLODInfo's Mesh (CCountedPointer<C3DMesh2>, Ego_r +0x1C) into the
// caller's CCountedPointer<const C3DMesh2>&. The body is ShareData expanded inline.
#include "fable_counted_pointer_inline.h"

class C3DMesh2;

template<typename T>
template<typename U>
inline CCountedPointer<T>& CCountedPointer<T>::operator=(const CCountedPointer<U>& other)
{
    ShareData(other.Data, other.Info);
    return *this;
}

template CCountedPointer<const C3DMesh2>& CCountedPointer<const C3DMesh2>::operator=(const CCountedPointer<C3DMesh2>&);
