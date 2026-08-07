#include "rebuild_abi.h"
// C3DAnimationInfo::IsLoading @ 0x00a23f30
// mov eax,[ecx+0xc]; test; je L; mov ecx,[eax+0x30]; test; je L; mov al,1; ret; L: xor al,al; ret
// True when the loader object exists and its pointer field at +0x30 is set.
struct CLoader { char pad00[0x30]; void *m_state; };
struct C3DAnimationInfo { char pad00[0xc]; CLoader *m_loader; bool IsLoading(); };
bool C3DAnimationInfo::IsLoading()
{
    CLoader *l = m_loader;
    if (l && l->m_state) return true;
    return false;
}