// ~C3DMeshLODInfo, retail 0x00A24470 (Ego_r ??1C3DMeshLODInfo@@UAE@XZ), and the scalar deleting
// destructor VC7.1 emits beside it, ??_GC3DMeshLODInfo@@UAEPAXI@Z 0x00A2B690 (vtable 0x0129CDA4
// slot 0). The body is empty in retail: only the member destructors run (FileData +0x24, then
// Mesh +0x1c), then the out-of-line ~CResource 0x009FC550 (a tail jump), as FableWin
// 0x0313AB20 also shows.
#define FABLE_RESOURCE_DESTRUCTOR_DECLARATION_ONLY
#include "engine/CMeshDataBank.h"

C3DMeshLODInfo::~C3DMeshLODInfo()
{
}
