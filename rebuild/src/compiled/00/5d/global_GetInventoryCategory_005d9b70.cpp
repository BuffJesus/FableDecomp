#include "rebuild_abi.h"
// GetInventoryCategory @ 0x005d9b70
struct GData { char pad[0x68]; int m_v; };
extern GData *g_invData_5d9b70;
extern "C" char FABLE_CDECL FableCheck_5d9b70();
extern "C" int FABLE_CDECL FableGetInv_5d9b70(){
    return FableCheck_5d9b70() ? g_invData_5d9b70->m_v : 0;
}