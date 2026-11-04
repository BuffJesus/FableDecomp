// ~C3DAnimationInfo, retail 0x00A25970 (Ego_r ??1C3DAnimationInfo@@UAE@XZ; callers ??_G 0x00A2BD73
// and the animation-info teardown 0x00A26EFB), and its scalar deleting destructor
// ??_GC3DAnimationInfo@@UAEPAXI@Z 0x00A2BD70 (vtable 0x0129CD64 slot 0). FableWin 0x0313E940 walks
// Data->Animations and checks that each animation is referenced only here (GetReferenceCount() == 1,
// a debug message compiled out of retail); retail keeps the bare in-order walk (the expanded
// _Rb_global::_M_increment), then Data's destructor and the inline ~CMeshDataBankEntry.
#include "engine/C3DAnimationInfo.h"

C3DAnimationInfo::~C3DAnimationInfo()
{
    if (!Data.IsNull())
    {
        for (_STL::map<unsigned long, CCountedPointer<C3DAnimation2> >::iterator animation =
                 Data->Animations.begin();
             animation != Data->Animations.end(); ++animation)
        {
        }
    }
}
