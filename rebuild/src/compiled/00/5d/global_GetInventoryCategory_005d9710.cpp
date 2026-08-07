#include "rebuild_abi.h"
// GetInventoryCategory @ 0x005d9710
struct GData { char pad[0x4c]; int m_v; };
extern GData *g_invData_5d9710;
extern "C" char FABLE_CDECL FableCheck_5d9710();
extern "C" int FABLE_CDECL FableGetInv_5d9710(){
    return FableCheck_5d9710() ? g_invData_5d9710->m_v : 0;
}