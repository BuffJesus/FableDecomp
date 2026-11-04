// ~C3DMeshInfo, retail 0x00A24930 (Ego_r ??1C3DMeshInfo@@UAE@XZ; only callers ??_G 0x00A2B6B3 and
// the info-buffer teardown 0x00A26E6C), and its scalar deleting destructor
// ??_GC3DMeshInfo@@UAEPAXI@Z 0x00A2B6B0 (vtable 0x0129CD3C slot 0). FableWin 0x0313BFD0 shows the
// source shape: when the mesh data is shared, every LOD still held is released (after a debug
// check that it is unloaded or unloadable, compiled out of retail), then MeshData's destructor
// and the inline ~CMeshDataBankEntry (vptr 0x0129CD28).
#include "engine/C3DMeshInfo.h"

C3DMeshInfo::~C3DMeshInfo()
{
    if (!MeshData.IsNull())
    {
        for (unsigned long lod = 0; lod < GetMaxLODs(); ++lod)
        {
            if (!MeshData->LODMeshes[lod].IsNull())
                MeshData->LODMeshes[lod].Reset(0);
        }
    }
}
