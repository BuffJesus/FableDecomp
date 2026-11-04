// ~CAnimationData, retail 0x00A26860 (Ego_r ??1CAnimationData@C3DAnimationInfo@@UAE@XZ), and its
// scalar deleting destructor ??_GCAnimationData@C3DAnimationInfo@@UAEPAXI@Z 0x00A2DB10 (vtable
// 0x0129CE50 slot 0; Ego_r names the slot ??_E, a weak alias under VC7.1). FableWin 0x031425B0
// clears AnimInfo and then only checks reference counts (compiled out of retail). The members
// then go in reverse order: FileData, SharedAnimation, and the Animations tree (erased through
// _M_erase 0x00A2B9D0, then the header node is freed), then the out-of-line ~CResource 0x009FC550.
#define FABLE_RESOURCE_DESTRUCTOR_DECLARATION_ONLY
#include "engine/C3DAnimationInfo.h"

// The Animations tree's destructor is the map<unsigned long, CCountedPointer<C3DAnimation2> >
// specialisation in fable_mesh_data_bank.h (STLport ~_Rb_tree: clear, then release the header node).

C3DAnimationInfo::CAnimationData::~CAnimationData()
{
    AnimInfo = 0;
}
