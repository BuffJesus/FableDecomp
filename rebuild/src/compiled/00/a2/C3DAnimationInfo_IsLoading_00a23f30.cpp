// C3DAnimationInfo::IsLoading, retail 0x00A23F30 (Ego_r ?IsLoading@C3DAnimationInfo@@QBE_NXZ,
// MeshDataBank.cpp). Caller 0x00694C3F on a PeekAnimation result: loading while the animation
// data still holds its file read (CAnimationData::FileData +0x30).
#include "engine/C3DAnimationInfo.h"

// Caller 0x00694C3F on a PeekAnimation result: loading while the animation data still holds
// its file read.
bool C3DAnimationInfo::IsLoading() const
{
    if (!Data.IsNull() && Data->FileData.Data)
        return true;
    return false;
}
