#include "rebuild_abi.h"
// CTCDoor::PostVandalismCrimeIfDestroyed @ 0x007305f0
// mov eax,[ecx+0x20]; test; je L; mov cl,[eax+0x38]; test cl; je L; mov al,1; ret; L: xor al,al; ret
// True only when the owned object exists and its byte flag at +0x38 is set.
struct CVandalTarget { char pad00[0x38]; unsigned char m_flag; };
struct CTCDoor { char pad00[0x20]; CVandalTarget *m_target; bool PostVandalismCrimeIfDestroyed(); };
bool CTCDoor::PostVandalismCrimeIfDestroyed()
{
    CVandalTarget *t = m_target;
    if (t && t->m_flag) return true;
    return false;
}