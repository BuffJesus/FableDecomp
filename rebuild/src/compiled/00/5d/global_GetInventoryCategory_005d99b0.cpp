#include "rebuild_abi.h"
// GetInventoryCategory @ 0x005d99b0
struct GData { char pad[0x64]; int m_v; };
extern GData *g_invData_5d99b0;
extern "C" char FABLE_CDECL FableCheck_5d99b0();
extern "C" int FABLE_CDECL FableGetInv_5d99b0(){
    return FableCheck_5d99b0() ? g_invData_5d99b0->m_v : 0;
}