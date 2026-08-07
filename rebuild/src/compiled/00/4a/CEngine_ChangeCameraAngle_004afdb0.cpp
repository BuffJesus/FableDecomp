#include "rebuild_abi.h"
// CEngine::ChangeCameraAngle @ 0x004afdb0
// push esi; mov esi,ecx; lea ecx,[esi+0x58]; call sub; mov byte[esi+0x8f],1; pop esi; ret
// Invoke a member on the embedded camera subobject at +0x58, then flag the change.
struct CCameraSub;
extern "C" void FABLE_FASTCALL FableCameraSubTick_4afdb0(CCameraSub *sub);
struct CEngine {
    char pad00[0x58];
    char m_cameraSub[0x8f-0x58];  // subobject region starting at +0x58
    unsigned char m_changed;      // +0x8f
    void ChangeCameraAngle();
};
void CEngine::ChangeCameraAngle()
{
    FableCameraSubTick_4afdb0((CCameraSub*)&m_cameraSub[0]);
    m_changed = 1;
}