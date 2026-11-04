// CMeshDataBank::Close, retail 0x00A26C50 (vtable 0x0129CE94 slot 5; Ego_r ?Close@CMeshDataBank@@UAEXXZ):
// release the entries (CloseMeshes 0x00A26900), then the asynchronous bank's own close
// (CBankFileAsync::Close 0x009D58A0, a tail jump).
#include "engine/CMeshDataBank.h"

void CMeshDataBank::Close()
{
    CloseMeshes();
    CBankFileAsync::Close();
}
