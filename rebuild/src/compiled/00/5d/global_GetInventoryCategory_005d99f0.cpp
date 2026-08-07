#include "rebuild_abi.h"
// GetInventoryCategory @ 0x005d99f0
struct GData { char pad[0x40]; int m_v; };
extern GData *g_invData_5d99f0;
extern "C" char FABLE_CDECL FableCheck_5d99f0();
extern "C" int FABLE_CDECL FableGetInv_5d99f0(){
    return FableCheck_5d99f0() ? g_invData_5d99f0->m_v : 0;
}