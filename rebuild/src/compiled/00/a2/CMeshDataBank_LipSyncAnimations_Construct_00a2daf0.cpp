// STLport _Construct<pair<const CCharString, CArray<CMeshDataBank::CLipSyncEntry> > >, retail
// 0x00A2DAF0 (31 bytes, no direct callers): the placement copy of a CMeshDataBank::LipSyncAnimations
// value (map<CCharString, CArray<CLipSyncEntry> >, retail +0x3C8) into a new tree node. The throw()
// placement new is null-checked; then the key's CCharString copy constructor 0x0099EC30 and the
// value's CArray copy (STLport vector copy constructor 0x00A2B970: allocate size*8, uninitialized
// copy) run on the node. Ego_r names the MS-STL counterparts
// (??$_Cons_val@...CLipSyncEntry@CMeshDataBank@@...@std@@YIX...).
// Lionhead built with __fastcall as the default convention (Ego_r's STL helpers are all YI), so
// the retail instantiation is __fastcall (pointer in ecx, value in edx). The shared
// fable_stlport_vector.h template carries no convention (everywhere else it is expanded inline),
// so this unit spells the instantiation as an explicit specialization with the template's own
// body and the retail convention.
#include <new>
#include "engine/CMeshDataBank.h"

typedef _STL::pair<const CCharString, CArray<CMeshDataBank::CLipSyncEntry> > CLipSyncAnimationValue;

namespace _STL {
template<> void FABLE_FASTCALL _Construct<CLipSyncAnimationValue, CLipSyncAnimationValue>(
    CLipSyncAnimationValue* __p, const CLipSyncAnimationValue& __val)
{
    new (__p) CLipSyncAnimationValue(__val);
}
}
