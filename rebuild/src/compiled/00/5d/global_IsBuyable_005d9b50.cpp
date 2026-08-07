#include "rebuild_abi.h"
// IsBuyable @ 0x005d9b50
struct GData { char pad[0x5d]; unsigned char m_v; };
extern GData *g_invData_5d9b50;
extern "C" char FABLE_CDECL FableCheck_5d9b50();
extern "C" char FABLE_CDECL FableIsBuyable_5d9b50(){
    return FableCheck_5d9b50() ? (char)g_invData_5d9b50->m_v : (char)0;
}